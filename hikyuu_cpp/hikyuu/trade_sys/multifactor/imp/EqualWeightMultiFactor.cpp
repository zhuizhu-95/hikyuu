/*
 *  Copyright (c) 2024 hikyuu.org
 *
 *  Created on: 2024-03-13
 *      Author: fasiondog
 */

#include "hikyuu/utilities/thread/algorithm.h"
#include "hikyuu/indicator/crt/PRICELIST.h"
#include "EqualWeightMultiFactor.h"

#if HKU_SUPPORT_SERIALIZATION
BOOST_CLASS_EXPORT(hku::EqualWeightMultiFactor)
#endif

namespace hku {

EqualWeightMultiFactor::EqualWeightMultiFactor() : MultiFactorBase("MF_EqualWeight") {}

EqualWeightMultiFactor::EqualWeightMultiFactor(const vector<Indicator>& inds, const StockList& stks,
                                               const KQuery& query, const Stock& ref_stk, int ic_n,
                                               bool spearman, int mode, bool save_all_factors)
: MultiFactorBase(inds, stks, query, ref_stk, "MF_EqualWeight", ic_n, spearman, mode,
                  save_all_factors) {}

vector<Indicator> EqualWeightMultiFactor::_calculate(
  const vector<vector<Indicator>>& all_stk_inds) {
    size_t days_total = m_ref_dates.size();
    size_t stk_count = m_stks.size();
    size_t ind_count = m_inds.size();

#if !MF_USE_MULTI_THREAD
    value_t null_value = Null<value_t>();
    vector<price_t> sumByDate(days_total);
    vector<size_t> countByDate(days_total);
    vector<Indicator> all_factors(stk_count);
    for (size_t si = 0; si < stk_count; si++) {
        memset(sumByDate.data(), 0, sizeof(price_t) * days_total);
        memset(countByDate.data(), 0, sizeof(size_t) * days_total);

        const auto& curStkInds = all_stk_inds[si];
        for (size_t di = 0; di < days_total; di++) {
            for (size_t ii = 0; ii < ind_count; ii++) {
                const auto& value = curStkInds[ii][di];
                if (!std::isnan(value)) {
                    sumByDate[di] += value;
                    countByDate[di] += 1;
                }
            }
        }

        // 均值权重
        for (size_t di = 0; di < days_total; di++) {
            sumByDate[di] = (countByDate[di] == 0) ? null_value : sumByDate[di] / countByDate[di];
        }

        all_factors[si] = PRICELIST(sumByDate);
        all_factors[si].name("IC");

        // 更新 discard
        for (size_t di = 0; di < days_total; di++) {
            if (!std::isnan(all_factors[si][di])) {
                all_factors[si].setDiscard(di);
                break;
            }
            if (di == days_total - 1 && std::isnan(all_factors[si][di])) {
                all_factors[si].setDiscard(di);
            }
        }
    }

    return all_factors;
#endif

    return parallel_for_index(0, stk_count, [&](size_t si) {
        vector<price_t> sumByDate(days_total);
        vector<size_t> countByDate(days_total);

        const auto& curStkInds = all_stk_inds[si];
        for (size_t di = 0; di < days_total; di++) {
            for (size_t ii = 0; ii < ind_count; ii++) {
                const auto& value = curStkInds[ii][di];
                if (!std::isnan(value)) {
                    sumByDate[di] += value;
                    countByDate[di] += 1;
                }
            }
        }

        // 均值权重
        for (size_t di = 0; di < days_total; di++) {
            sumByDate[di] =
              (countByDate[di] == 0) ? Null<value_t>() : sumByDate[di] / countByDate[di];
        }

        Indicator ret = PRICELIST(sumByDate);
        ret.name("IC");

        // 更新 discard
        for (size_t di = 0; di < days_total; di++) {
            if (!std::isnan(ret[di])) {
                ret.setDiscard(di);
                break;
            }
            if (di == days_total - 1 && std::isnan(ret[di])) {
                ret.setDiscard(di);
            }
        }
        return ret;
    });
}

MultiFactorPtr HKU_API MF_EqualWeight() {
    return make_shared<EqualWeightMultiFactor>();
}

MultiFactorPtr HKU_API MF_EqualWeight(const IndicatorList& inds, const StockList& stks,
                                      const KQuery& query, const Stock& ref_stk, int ic_n,
                                      bool spearman, int mode, bool save_all_factors) {
    return make_shared<EqualWeightMultiFactor>(inds, stks, query, ref_stk, ic_n, spearman, mode,
                                               save_all_factors);
}

}  // namespace hku
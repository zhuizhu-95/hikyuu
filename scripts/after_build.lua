import("core.platform.platform")
import("core.base.option")

function main(target)
    if target:isphony() then
        return 
    end

    local targetname = target:name()
    local targetfile = target:targetfile()

    if "sqlite3" == targetname then
        os.cp(targetfile, '$(projectdir)/hikyuu/cpp')
        return
    end

    if "hikyuu" == targetname then
        os.cp(targetfile, '$(projectdir)/hikyuu/cpp')
        if is_plat("windows") then
            os.cp("$(env BOOST_LIB)/boost_python3*.dll", '$(projectdir)/hikyuu/cpp')
            os.cp("$(env BOOST_LIB)/boost_serialization*.dll", '$(projectdir)/hikyuu/cpp')
            if is_mode("release") then
                os.cp("$(projectdir)/hikyuu_extern_libs/pkg/hdf5.pkg/lib/$(mode)/$(plat)/$(arch)/*.dll", '$(projectdir)/hikyuu/cpp')
            else
                os.cp("$(projectdir)/hikyuu_extern_libs/pkg/hdf5_D.pkg/lib/$(mode)/$(plat)/$(arch)/*.dll", '$(projectdir)/hikyuu/cpp')
            end
            os.cp("$(projectdir)/hikyuu_extern_libs/pkg/mysql.pkg/lib/$(mode)/$(plat)/$(arch)/*.dll", '$(projectdir)/hikyuu/cpp')
            return
        end

        if is_plat("linux") then
            os.cp("$(env BOOST_LIB)/libboost_python3*.so.*", '$(projectdir)/hikyuu/cpp')
            os.cp("$(env BOOST_LIB)/libboost_serialization*.so.*", '$(projectdir)/hikyuu/cpp')
            return
        end

        if is_plat("macosx") then
            os.cp("$(env BOOST_LIB)/libboost_python3*.dylib", '$(projectdir)/hikyuu/cpp')
            os.cp("$(env BOOST_LIB)/libboost_serialization*.dylib", '$(projectdir)/hikyuu/cpp')
            return
        end

        return
    end

    if "_hikyuu" == targetname then
        os.cp(targetfile, '$(projectdir)/hikyuu/cpp')
        return
    end

end
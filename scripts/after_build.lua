import("core.platform.platform")
import("core.base.option")

function main(target)
    if target:isphony() then
        return 
    end

    local targetname = target:name()
    local targetfile = target:targetfile()

    if "sqlite3" == targetname then
        os.cp(targetfile, '$(projectdir)/hikyuu')
        return
    end

    if "hikyuu" == targetname then
        os.cp(targetfile, '$(projectdir)/hikyuu')
        if is_plat("windows") then
            os.cp("$(env BOOST_LIB)/boost_python3*.dll", '$(projectdir)/hikyuu')
            os.cp("$(env BOOST_LIB)/boost_serialization*.dll", '$(projectdir)/hikyuu')
            if is_mode("release") then
                os.cp("$(projectdir)/hikyuu_extern_libs/pkg/hdf5.pkg/lib/$(mode)/$(plat)/$(arch)/*.dll", '$(projectdir)/hikyuu')
            else
                os.cp("$(projectdir)/hikyuu_extern_libs/pkg/hdf5_D.pkg/lib/$(mode)/$(plat)/$(arch)/*.dll", '$(projectdir)/hikyuu')
            end
            os.cp("$(projectdir)/hikyuu_extern_libs/pkg/mysql.pkg/lib/$(mode)/$(plat)/$(arch)/*.dll", '$(projectdir)/hikyuu')
            return
        end

        if is_plat("linux") then
            os.cp("$(env BOOST_LIB)/libboost_python3*.so.*", '$(projectdir)/hikyuu')
            os.cp("$(env BOOST_LIB)/libboost_serialization*.so.*", '$(projectdir)/hikyuu')
            return
        end

        if is_plat("macosx") then
            os.cp("$(env BOOST_LIB)/libboost_python3*.dylib", '$(projectdir)/hikyuu')
            os.cp("$(env BOOST_LIB)/libboost_serialization*.dylib", '$(projectdir)/hikyuu')
            return
        end

        return
    end

    if "core" == targetname then
        os.cp(targetfile, '$(projectdir)/hikyuu')
        return
    end

end
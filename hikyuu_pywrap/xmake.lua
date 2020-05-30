option("boost-python-suffix")
    set_default("3X")
    set_showmenu(true)
    set_category("hikyuu")
    set_description("Set suffix of libboost_python. ",
                    "Boost.python 1.67 later use 3x like libboost_python35, ",
                    "but older is libboost_python3",
                    "    - 3X autocheck for 35, 36, 37, 3x")
option_end()

package("pybind11")
    set_homepage("https://github.com/pybind/pybind11")
    set_description("Seamless operability between C++11 and Python.")

    set_urls("https://github.com/pybind/pybind11/archive/v$(version).zip",
             "https://github.com/pybind/pybind11.git")

    add_versions("2.5.0", "1859f121837f6c41b0c6223d617b85a63f2f72132bae3135a2aa290582d61520")

    on_install(function (package)
        os.cp("include", package:installdir())
    end)

    on_test(function (package)
        if is_plat("windows") then
            local pydir = os.iorun("python -c \"import sys; print(sys.executable)\"")
            local py_include_dir = path.directory(pydir) .. "/include"
            assert(package:has_cxxfuncs("pybind11::globals()", {includes = "pybind11/pybind11.h", configs = {includedirs={py_include_dir, package:installdir().."/include"}, languages = "c++11"}}))
            return
        end

        py_include_dir = try { function () return os.iorun("python3-config --includes"):trim() end }
        local py_lib_dir = os.iorun("python3-config --prefix"):trim() .. "/lib"
        local out, err = os.iorun("python3 --version")
        local ver = (out .. err):trim()
        local pylib = format("python%s.%sm", string.sub(ver, 8, 8), string.sub(ver, 10, 10))
        assert(package:has_cxxfuncs("pybind11::globals()", {includes = "pybind11/pybind11.h", configs = {cxflags=py_include_dir, links = pylib, languages = "c++11"}}))
    end)
package_end()

add_includedirs("../hikyuu_cpp")
add_requires("pybind11")

target("core")
    set_kind("shared")
    if is_mode("debug") then 
        set_default(false) --会默认禁用这个target的编译，除非显示指定xmake build _hikyuu才会去编译，但是target还存在，里面的files会保留到vcproj
        --set_enable(false) --set_enable(false)会彻底禁用这个target，连target的meta也不会被加载，vcproj不会保留它
    end
    if is_plat("windows") then
        set_filename("core.pyd")
    else 
        set_filename("core.so")
    end

    add_packages("fmt", "spdlog", "pybind11")
    add_deps("hikyuu")

    -- Can't use static boost.python lib, the function that using 'arg' will load failed!
    --add_defines("BOOST_PYTHON_STATIC_LIB")

    if is_plat("windows") then
        add_defines("HKU_API=__declspec(dllimport)")
        add_cxflags("/utf-8")
        add_cxflags("-wd4566")
        add_cxflags("-wd4251")
    end

    local cc = get_config("cc")
    local cxx = get_config("cxx")
    if (cc and string.find(cc, "clang")) or (cxx and string.find(cxx, "clang")) then
        add_cxflags("-Wno-error=parentheses-equality -Wno-error=missing-braces")
    end

    add_files("./**.cpp")

    on_load(function(target)
        import("lib.detect.find_tool")
        if is_plat("windows") then
            -- detect installed python3
            local python = assert(find_tool("python", {version = true}), "python not found, please install it first! note: python version must > 3.0")
            assert(python.version > "3", python.version .. " python version must > 3.0, please use python3.0 or later!")

            -- find python include and libs directory
            local pydir = os.iorun("python -c \"import sys; print(sys.executable)\"")
            pydir = path.directory(pydir)
            target:add("includedirs", pydir .. "/include")
            target:add("linkdirs", pydir .. "/libs")
            return
        end

        target:add("rpathdirs", "$ORIGIN", "$ORIGIN/lib", "$ORIGIN/../lib")
        
        if is_plat("macosx") then
            local libdir = os.iorun("python3-config --prefix"):trim() .. "/lib"
            target:add("linkdirs", libdir)
            local out, err = os.iorun("python3 --version")
            local ver = (out .. err):trim()
            local python_lib = format("python%s.%sm", string.sub(ver,8,8), string.sub(ver,10,10))
            target:add("links", python_lib)
        end

        -- get python include directory.
        local pydir = try { function () return os.iorun("python3-config --includes"):trim() end }
        assert(pydir, "python3-config not found!")
        target:add("cxflags", pydir)

        -- get suffix configure for link libboost_pythonX.so
        local suffix = get_config("boost-python-suffix")
        if suffix == nil then
            raise("You need to config --boost-python-suffix specify libboost_python suffix")
        end

        suffix = string.upper(suffix)
        if suffix == "3X" then
            local out, err = os.iorun("python3 --version")
            local ver = (out .. err):trim()
            local boost_python_lib = "boost_python"..string.sub(ver,8,8)..string.sub(ver,10,10)
            target:add("links", boost_python_lib)
        else
            target:add("links", "boost_python"..suffix)
        end
    end)

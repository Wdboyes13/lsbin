fs = import('fs')

conf = configuration_data()
conf.set_quoted('LSBIN_PATH', meson.project_build_root() / meson.project_name())
conf.set_quoted('TEST_FILES_PATH', meson.current_source_dir() / 'files')

configh = configure_file(
    input: 'config.h.in',
    output: 'test_config.h',
    configuration: conf,
)

config_inc = include_directories(
    fs.parent(
        fs.relative_to(
            meson.project_build_root() / configh.full_path(),
            meson.current_source_dir(),
        ),
    ),
)

testinc = [
    include_directories('.'), 
    config_inc, 
    include_directories('../include/third_party'),
    include_directories('../include')
]

testlib_tgt = shared_library('testlib', sources: 'test_lib.cc', include_directories: testinc)

testlib = declare_dependency(link_with: testlib_tgt, include_directories: testinc)

tests = {
    'PE Test': 'pe_test.cc',
    'ELF Test': 'elf_test.cc',
    'MachO Test': 'macho_test.cc',
    'FAT MachO Test': 'fatmacho_test.cc',
}

foreach name, src : tests
    test(name, executable(
        src.replace('.cc', ''),
        src,
        dependencies: testlib,
        install: false
    ))
endforeach
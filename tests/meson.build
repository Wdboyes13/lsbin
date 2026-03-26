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

testinc = [include_directories('.'), config_inc]

testlib_tgt = shared_library('testlib', sources: 'test_lib.cc', include_directories: testinc)

testlib = declare_dependency(link_with: testlib_tgt, include_directories: testinc)

test(
    'PE Test',
    executable(
        'pe_test',
        'pe_test.cc',
        dependencies: testlib,
        install: false,
    ),
)

test(
    'ELF Test',
    executable('elf_test', 'elf_test.cc', dependencies: testlib, install: false),
)

test(
    'MachO Test',
    executable('macho_test', 'macho_test.cc', dependencies: testlib, install: false),
)

test(
    'FAT MachO Test',
    executable(
        'fat_macho_test',
        'fatmacho_test.cc',
        dependencies: testlib,
        install: false,
    ),
)

project(
    'lsbin',
    'cpp',
    default_options: ['cpp_std=c++23'],
    meson_version: '>=1.4.0',
)

compiler = meson.get_compiler('cpp')
build_type = get_option('buildtype')

cppargs = []

if compiler.get_id() != 'msvc'
    cppargs += ['-fms-extensions']
endif

if build_type != 'debug' and build_type != 'debugoptimized'
    if compiler.get_id() != 'msvc'
        cppargs += ['-DNDEBUG']
    else
        cppargs += ['/DNDEBUG']
    endif
endif

sources = files(
    'src/formats/elf32.cc',
    'src/formats/elf64.cc',
    'src/formats/macho.cc',
    'src/formats/pe.cc',

    'src/file.cc',
    'src/printer.cc',
    'src/image.cc',
    'src/args.cc',

    'src/main.cc',
)

executable(
    meson.project_name(),
    sources,
    cpp_args: cppargs,
    include_directories: [
        include_directories('include'),
        include_directories('include/third_party')
    ],
)

subdir('tests')

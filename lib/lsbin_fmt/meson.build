lsbin_fmt_includes = [
    include_directories('include'),
    include_directories('include/third_party')
]

lsbin_fmt_lib = static_library(
    'lsbin_fmt',
    sources: files('src/file.cc'),
    include_directories: lsbin_fmt_includes,
    install: false
)

lsbin_fmt = declare_dependency(
    link_with: lsbin_fmt_lib,
    include_directories: lsbin_fmt_includes
)
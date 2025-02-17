# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) 2017-2019 Intel Corporation

pkgconf = find_program('pkg-config', 'pkgconf', required: false)
check_symbols = find_program('check-symbols.sh')
ldflags_ibverbs_static = find_program('options-ibverbs-static.sh')
objdump = find_program('objdump', 'llvm-objdump')

python3_required_modules = []
if host_machine.system() != 'windows'
    python3_required_modules = ['elftools']
endif
python3 = import('python').find_installation('python3', required: false, modules: python3_required_modules)
if python3.found()
    py3 = [python3]
else
    py3 = ['meson', 'runpython']
endif
echo = py3 + ['-c', 'import sys; print(*sys.argv[1:])']
list_dir_globs = py3 + files('list-dir-globs.py')
map_to_win_cmd = py3 + files('map_to_win.py')
sphinx_wrapper = py3 + files('call-sphinx-build.py')
get_cpu_count_cmd = py3 + files('get-cpu-count.py')
get_numa_count_cmd = py3 + files('get-numa-count.py')
binutils_avx512_check = (py3 + files('binutils-avx512-check.py') +
                        [objdump] + cc.cmd_array())

# select library and object file format
pmdinfo = py3 + files('gen-pmdinfo-cfile.py') + [meson.current_build_dir()]
pmdinfogen = py3 + files('pmdinfogen.py')
if host_machine.system() == 'windows'
    if cc.get_id() == 'gcc'
        pmdinfo += 'ar'
    else
        pmdinfo += 'llvm-ar'
    endif
    pmdinfogen += 'coff'
elif host_machine.system() == 'freebsd'
    pmdinfo += 'llvm-ar'
    pmdinfogen += 'elf'
else
    pmdinfo += 'ar'
    pmdinfogen += 'elf'
endif


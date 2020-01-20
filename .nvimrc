"------------- vim-quickui settings  ------------------"
call quickui#menu#install('&C', [
            \ [ 'make(&debug)', 'update | w | :Floating cd build; cmake -DCMAKE_BUILD_TYPE=Debug ../; cd ..; ln -s build/compile_commands.json .; cd build/; make;' ],
            \ [ 'run(debu&g)', "update | w | :Floating debug/captain-ballard" ],
            \ [ '&make(release)', "update | w | :Floating cd build; cmake -DCMAKE_BUILD_TYPE=Release ../; cd ..; ln -s build/compile_commands.json .; cd build/; make;" ],
            \ [ '&run(release)', "update | w | :Floating release/captain-ballard" ],
            \ [ '&clean-project', "update | w | :Floating rm -rfv build/**; rm -rfv debug/**; rm -rfv release/**; rm -rfv compile_commands.json" ],
            \ [ "--", '' ],
            \ ], '<auto>', 'c')

"---------- clang-format settings --------""
let g:clang_format#code_style="webkit"
let g:clang_format#auto_format=1
let g:clang_format#auto_formatexpr=1
let g:clang_format#auto_format_on_insert_leave=1
"" Toggle auto formatting:
"nmap <Leader>C :ClangFormatAutoToggle<CR>
"-------------------------------------------------------"

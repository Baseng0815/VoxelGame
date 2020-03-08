let SessionLoad = 1
if &cp | set nocp | endif
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/projects/VoxelGame
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
argglobal
%argdel
edit include/Camera.h
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 28 + 30) / 61)
exe 'vert 1resize ' . ((&columns * 118 + 118) / 236)
exe '2resize ' . ((&lines * 28 + 30) / 61)
exe 'vert 2resize ' . ((&columns * 118 + 118) / 236)
exe 'vert 3resize ' . ((&columns * 117 + 118) / 236)
argglobal
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
14
normal! zo
let s:l = 35 - ((21 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
35
normal! 09|
wincmd w
argglobal
if bufexists("src/ChunkCreateSystem.cpp") | buffer src/ChunkCreateSystem.cpp | else | edit src/ChunkCreateSystem.cpp | endif
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
28
normal! zo
32
normal! zo
33
normal! zo
34
normal! zo
183
normal! zo
let s:l = 182 - ((11 * winheight(0) + 14) / 28)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
182
normal! 0
wincmd w
argglobal
if bufexists("include/ChunkRenderSystem.h") | buffer include/ChunkRenderSystem.h | else | edit include/ChunkRenderSystem.h | endif
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
12
normal! zo
let s:l = 24 - ((23 * winheight(0) + 28) / 57)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
24
normal! 024|
wincmd w
3wincmd w
exe '1resize ' . ((&lines * 28 + 30) / 61)
exe 'vert 1resize ' . ((&columns * 118 + 118) / 236)
exe '2resize ' . ((&lines * 28 + 30) / 61)
exe 'vert 2resize ' . ((&columns * 118 + 118) / 236)
exe 'vert 3resize ' . ((&columns * 117 + 118) / 236)
tabnext 1
badd +35 include/Camera.h
badd +40 src/ChunkCreateSystem.cpp
badd +0 include/ChunkRenderSystem.h
badd +1 include/BiomeInfo.h
badd +30 include/Application.h
badd +12 src/Application.cpp
badd +31 include/ChunkCreateSystem.h
badd +31 include/Components/ChunkComponent.h
badd +4 include/Vertex.h
badd +13 src/Components/ChunkComponent.cpp
badd +3 include/TextureAtlas.h
badd +1 include/Definitions.h
badd +38 src/Font.cpp
badd +15 src/MasterRenderer.cpp
badd +42 include/MasterRenderer.h
badd +14 src/ChunkRenderSystem.cpp
badd +12 include/System.h
badd +18 include/Player.h
badd +89 ./src/Camera.cpp
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToOSc
set winminheight=1 winminwidth=1
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
nohlsearch
let g:this_session = v:this_session
let g:this_obsession = v:this_session
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :

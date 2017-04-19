" Set colorscheme
set t_Co=256
syntax on
colorscheme blues

" Show line numbers
set number

" Set tab spacing
set autoindent noexpandtab tabstop=4 shiftwidth=4

" Show searches dynamically
set incsearch
set hlsearch

" Remap pane navigation to just ctrl+direction
nnoremap <C-J> <C-W><C-J>
nnoremap <C-K> <C-W><C-K>
nnoremap <C-L> <C-W><C-L>
nnoremap <C-H> <C-W><C-H>

" Highlight current line
set cursorline

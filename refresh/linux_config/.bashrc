# Colorize terminal prompt
PS1='\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '

# display all files in current directory in detailed mode
alias ll="ls -al --color"

# Use ctrl+l to clear terminal
bind -m vi-insert "\C-l":clear-screen

function drive() {
	~/mr_robot/tools/control/write $1,$2#
}

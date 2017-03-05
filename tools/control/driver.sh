#!/bin/bash

function forward() {
	~/Code/C/write $1,$1#
}

function stop() {
	~/Code/C/write 0,0#
}

function reverse() {
	~/Code/C/write -$1,-$1#
}

function turn() {
	~/Code/C/write $1,$2#
}

function right() {
	~/Code/C/write $1,-$1#
}

function left() {
	~/Code/C/write -$1,$1#
}


#!/bin/bash

set -e

OUTPUT="uwu_UWU"

function uwuifyDir() {
	IN="/usr/share/locale/${1}/LC_MESSAGES"
	OUT="/usr/share/locale/${OUTPUT}/LC_MESSAGES"
	
	mkdir -p "${OUT}"
	
	for FILE in ${IN}/*; do
		BASENAME=$(basename ${FILE})
		OUTFILE="${OUT}/${BASENAME}"
		echo "${FILE} -> ${OUTFILE}"
		
		./lowocale ${FILE} ${OUTFILE}
	done
}

uwuifyDir en
uwuifyDir en_GB

#!/bin/bash

set -e

if [ "$#" != 1 ]; then
	echo "usage: ${0} <locale name>"
	exit 1
fi

LOCALE="${1}"
OUTPUT="uwu_UWU"
LOCALE_CONF="/etc/locale.conf"

echo "uwuifying ${LOCALE}"

function uwuifyDir() {
	IN="/usr/share/locale/${1}/LC_MESSAGES"
	OUT="/usr/share/locale/${OUTPUT}/LC_MESSAGES"
	
	mkdir -p "${OUT}"

	if [ -d "${IN}" ]; then
		for FILE in ${IN}/*; do
			BASENAME=$(basename ${FILE})
			OUTFILE="${OUT}/${BASENAME}"
			echo "${FILE} -> ${OUTFILE}"
			
			./lowocale ${FILE} ${OUTFILE}
		done
	fi
}

eval $(LANG="${LOCALE}" locale)

if [ ! -f "./lowocale" ]; then
	cc lowocale.c -o lowocale
	chmod +x lowocale
fi

cp "/usr/share/i18n/locales/${LANG}" "/usr/share/i18n/locales/${OUTPUT}"

if [ "${LOCALE}" == "en_US" ]; then
	uwuifyDir en
	uwuifyDir en_GB
fi

uwuifyDir ${LOCALE}
uwuifyDir ${LC_MESSAGES}

cp "${LOCALE_CONF}" "${LOCALE_CONF}.old"
echo "LANG=${OUTPUT}" > "${LOCALE_CONF}"

echo "done! uwu"
echo "you'll want to reload your locales (a simple google search should help),"
echo "then you should have an uwu locale available!"

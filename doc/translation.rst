============
Translations
============

Update dungeons.pot (template file)::

    make intl-extract

New language::

    mkdir -p locale/sv/LC_MESSAGES
    msginit -l sv -o locale/sv/LC_MESSAGES/dungeons.po -i locale/dungeons.pot

Update existing language::

    msgmerge -s -U locale/sv/LC_MESSAGES/dungeons.po locale/dungeons.pot

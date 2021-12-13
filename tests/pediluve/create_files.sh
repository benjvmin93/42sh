touch ' '
chmod 0644 ' '

touch '\'
chmod 0644 '\'

touch -- "--"
chmod 0644 -- '--'

touch '|'
chmod 0644 '|'

touch '"'
chmod 0644 '"'

touch "'"
chmod 0644 "'"

touch '# Exams are fun!'
chmod 0644 '# Exams are fun!'

touch ';`kill -9 0`'
chmod 0644 ';`kill -9 0`'

touch -- '--''$i*'"'"'"''\'
chmod 0644  -- '--''$i*'"'"'"''\'

for i in $(seq 50)
do
    mkdir "$i"
    cd "$i"
done

touch 'farfaraway'
chmod 0644 'farfaraway'

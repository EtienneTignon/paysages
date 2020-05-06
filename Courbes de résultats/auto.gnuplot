set datafile separator ','
plot for [col=2:5] 'ecart-type_resultats 6.csv' using 1:col with lines title columnheader

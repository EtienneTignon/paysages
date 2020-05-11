set datafile separator ','
plot for [col=2:4] 'courbe.csv' using 1:col with lines title columnheader lw 2

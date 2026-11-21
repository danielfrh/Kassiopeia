#!/usr/bin/env python3

# Study magnitude of electric field in z-direction Emag(z) at running z-coordinate as calculated and
# both stored in csv files by application FieldCalculator_Electric.
#
# The script takes as start parameter the location of the two files to be compared as input.
# Here, the first input file is taken as reference potential U_1(z)!
#
# >> Start on terminal with:
# 'python3 FieldCalculatorElectricHelper-1dim-CompareEmag.py fileWithE1.csv fileWithE2.csv'
#
# The output is of this script is a graph illustrating the relative error of the electric
# potential:
# dEmagrel(z) = abs( Emag_2(z) - Emag_1(z) ) / abs( Emag_1(z) )
#
# Please note:
#
# * Only the relative error will be evaluated in the following script.
# * The absolute error measures the size of the difference between the estimated / measured value
#   and the true value (reference value): dE_absolute = abs( x_measured - x_ref )
# * The relative error expresses this difference as a percentage of the true / reference value:
#   dE_relative = dE_absolute / abs( x_ref )
# * The script can easily be changed to plot the absolute error, too (cf. l. 80f.).
#
# Since potential and field quentities can be negative, the corresponding absolute value will be
# taken in order to obtain an absolute value for the error.
#
# From:
# https://www.geeksforgeeks.org/maths/absolute-error-and-relative-error-formula-and-equation/
# https://www.vedantu.com/maths/absolute-and-relative-error


import pandas as pd
import matplotlib.pyplot as plt
import sys
# for int64
import numpy as np

dimension = 1


# Pfad zur Textdatei festlegen
dateipfad1 = sys.argv[1]
dateipfad2 = sys.argv[2]

# felder vergleichen x, y, z
# maximum,
# relativer fehler
# maximaler fehler
#ort/ koordinaten des maximalen fehlers

if len(sys.argv) != 3:
    print("Aufruf: python3 script.py datei1.csv datei2.csv")
    sys.exit(1)
for eachArg in sys.argv:
        print(eachArg)

# Defining the columns to read
titles = ["Id","x","y","z","Ex","Ey","Ez","Emag","Pot"]

# Daten einlesen
# header=0  bedeutet: Die erste Zeile der Datei (Index 0) enthält die Spaltennamen.
# names=titles  bedeutet: Ignoriere Spaltennamen aus der Datei und verwende diese hier.

try:
    df1 = pd.read_csv(dateipfad1, sep='\t', header=0, index_col="Id", usecols=titles, dtype={"Id": np.int64, "x": float, "y": float, "z": float, "Ex": float, "Ey": float, "Ez": float, "Emag": float, "Pot": float}, skipinitialspace=True)
    print( df1 )
except ValueError as e:
    print(f"Error: {e}")

try:
    df2 = pd.read_csv(dateipfad2, sep='\t', header=0, index_col="Id", usecols=titles, dtype={"Id": np.int64, "x": float, "y": float, "z": float, "Ex": float, "Ey": float, "Ez": float, "Emag": float, "Pot": float}, skipinitialspace=True)
    print( df2 )
except ValueError as e:
    print(f"Error: {e}")

cols = ["x","y","z","Ex","Ey","Ez","Emag","Pot"]
j = df1[cols].join(df2[cols], how="inner", lsuffix="_1", rsuffix="_2")

# Definition of relative and absolute error explained on top
j["dEmagAbs"] = (j["Emag_2"] - j["Emag_1"]).abs()
j["dEmagRel"] = ( j["dEmagAbs"] / ( j["Emag_1"]).abs() )

# 2) Plot: z auf x-Achse, Differenz auf y-Achse
# robust: nach z sortieren (falls Id nicht monoton in z ist)
plot_df = j.sort_values("z_1")

plt.figure(figsize=(7,4))
plt.plot(plot_df["z_1"], plot_df["dEmagRel"], lw=1)
plt.xlabel("z")
plt.ylabel("|ΔEmag|/|Emag_1| = |Emag_2 - Emag_1|/|Emag_1|")
plt.title("Relative Error ΔEmagRel against z-coordinate")
plt.grid(True)
plt.tight_layout()
plt.show()

# 3) Maximale Differenz finden + zugehörige Spalten ausgeben
# meist sinnvoll: maximale *Betrags*-Differenz
idx = j["dEmagRel"].idxmax()
# Id (Index)
row = j.loc[idx]

print("\nMaximal relative error |ΔEmag|/dEmag_1 found at Id =", idx)
print("z =", row["z_1"], " (x,y,z) = (", row["x_1"], ",", row["y_1"], ",", row["z_1"], ")")
print("Emag_1 =", row["Emag_1"])
print("Emag_2 =", row["Emag_2"])
print("|ΔEmag/Emag_1|=", abs(row["dEmagRel"]))

# Alle zugehörigen Feldkomponenten:
print(row[["x_1","y_1","z_1","Ex_1","Ey_1","Ez_1","Emag_1","Pot_1","Ex_2","Ey_2","Ez_2","Emag_2","Pot_2","dEmagRel"]])

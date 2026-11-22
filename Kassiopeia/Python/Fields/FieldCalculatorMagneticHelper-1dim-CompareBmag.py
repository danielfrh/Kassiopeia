#!/usr/bin/env python3

# Study magnitude of magnetic field in z-direction Bmag(z) at running z-coordinate as calculated and
# both stored in csv files by application FieldCalculator_Magnetic.
#
# The script takes as start parameter the location of the two files to be compared as input.
# Here, the first input file is taken as reference potential U_1(z)!
#
# >> Start on terminal with:
# 'python3 FieldCalculatorMagneticHelper-1dim-CompareEmag.py fileWithBmag1.csv fileWithBmag2.csv'
#
# The output is of this script is a graph illustrating the relative error of the electric
# potential:
# dBmagRel(z) = abs( Bmag_2(z) - Bmag_1(z) ) / abs( Bmag_1(z) )
#
# Please note:
#
# * Only the relative error will be plotted in the following script, but the script
#   can easily be changed to plot the absolute error, too (cf. l. 80f.).
# * The absolute error measures the size of the difference between the estimated / measured value
#   and the true value (reference value): dE_absolute = abs( x_measured - x_ref )
# * The relative error expresses this difference as a percentage of the true / reference value:
#   dE_relative = dE_absolute / abs( x_ref )
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
titles = ["Id","x","y","z","Bx","By","Bz","Bmag"]

# Daten einlesen
# header=0  bedeutet: Die erste Zeile der Datei (Index 0) enthält die Spaltennamen.
# names=titles  bedeutet: Ignoriere Spaltennamen aus der Datei und verwende diese hier.

try:
    df1 = pd.read_csv(dateipfad1, sep='\t', header=0, index_col="Id", usecols=titles, dtype={"Id": np.int64, "x": float, "y": float, "z": float, "Bx": float, "By": float, "Bz": float, "Bmag": float}, skipinitialspace=True)
    print( df1 )
except ValueError as e:
    print(f"Error: {e}")

try:
    df2 = pd.read_csv(dateipfad2, sep='\t', header=0, index_col="Id", usecols=titles, dtype={"Id": np.int64, "x": float, "y": float, "z": float, "Bx": float, "By": float, "Bz": float, "Bmag": float}, skipinitialspace=True)
    print( df2 )
except ValueError as e:
    print(f"Error: {e}")

cols = ["x","y","z","Bx","By","Bz","Bmag"]
j = df1[cols].join(df2[cols], how="inner", lsuffix="_1", rsuffix="_2")

# Definition of relative and absolute error explained on top
j["dBmagAbs"] = (j["Bmag_2"] - j["Bmag_1"]).abs()
j["dBmagRel"] = ( j["dBmagAbs"] / ( j["Bmag_1"]).abs() )

# 2) Plot: z auf x-Achse, Differenz auf y-Achse
# robust: nach z sortieren (falls Id nicht monoton in z ist)
plot_df = j.sort_values("z_1")

plt.figure(figsize=(7,4))
plt.plot(plot_df["z_1"], plot_df["dBmagRel"], lw=1)
plt.xlabel("z")
plt.ylabel("|ΔBmag|/|Bmag_1| = |Bmag_2 - Bmag_1|/|Bmag_1|")
plt.title("Relative Error ΔBmagRel against z-coordinate")
plt.grid(True)
plt.tight_layout()
plt.show()

# 3) Maximale Differenz finden + zugehörige Spalten ausgeben
idx = j["dBmagRel"].idxmax()
# Id (Index)
row = j.loc[idx]

print("\nMaximal relative error |ΔBmag|/dBmag_1 found at Id =", idx)
print("z =", row["z_1"], " (x,y,z) = (", row["x_1"], ",", row["y_1"], ",", row["z_1"], ")")
print("Bmag_1 =", row["Bmag_1"])
print("Bmag_2 =", row["Bmag_2"])
print("|ΔBmag|/|Bmag_1|=", row["dBmagRel"])

# Alle zugehörigen Feldkomponenten:
print(row[["x_1","y_1","z_1","Bx_1","By_1","Bz_1","Bmag_1","Bx_2","By_2","Bz_2","Bmag_2","dBmagRel"]])

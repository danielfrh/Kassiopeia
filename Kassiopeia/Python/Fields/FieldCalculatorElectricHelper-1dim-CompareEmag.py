#!/usr/bin/env python3

# Study field lines on lines as calculated and stored by Application FieldCalculator
# with relative w.r.t. first input 

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
    print("Aufruf: python script.py datei1.csv datei2.csv")
    sys.exit(1)
for eachArg in sys.argv:
        print(eachArg)

# Defining the columns to read
titles = ["Id","x","y","z","Bx","By","Bz","absB"]

# Daten einlesen
# header=0  bedeutet: Die erste Zeile der Datei (Index 0) enthält die Spaltennamen.
# names=titles  bedeutet: Ignoriere Spaltennamen aus der Datei und verwende diese hier.

try:
    df1 = pd.read_csv(dateipfad1, sep='\t', header=0, index_col="Id", usecols=titles, dtype={"Id": np.int64, "x": float, "y": float, "z": float, "Bx": float, "By": float, "Bz": float, "absB": float}, skipinitialspace=True)
    print( df1 )
except ValueError as e:
    print(f"Error: {e}")

try:
    df2 = pd.read_csv(dateipfad2, sep='\t', header=0, index_col="Id", usecols=titles, dtype={"Id": np.int64, "x": float, "y": float, "z": float, "Bx": float, "By": float, "Bz": float, "absB": float}, skipinitialspace=True)
    print( df2 )
except ValueError as e:
    print(f"Error: {e}")

cols = ["x","y","z","Bx","By","Bz","absB"]
j = df1[cols].join(df2[cols], how="inner", lsuffix="_1", rsuffix="_2")

# Relativer Fehler (hier: (df1 - df2) / df1 )
j["dBzBz1"] = (j["Bz_1"] - j["Bz_2"])/j["Bz_1"]

# 2) Plot: z auf x-Achse, Differenz auf y-Achse
# robust: nach z sortieren (falls Id nicht monoton in z ist)
plot_df = j.sort_values("z_1")

plt.figure(figsize=(7,4))
plt.plot(plot_df["z_1"], plot_df["dBzBz1"], lw=1)
plt.xlabel("z")
plt.ylabel("ΔBz/Bz_1 = (Bz_1 - Bz_2)/Bz_1")
plt.title("Relative Error ΔBz/Bz_1 over z")
plt.grid(True)
plt.tight_layout()
plt.show()

# 3) Maximal relative error
# meist sinnvoll: maximale *Betrags*-Differenz
idx = j["dBzBz1"].abs().idxmax()        # Id (Index)
row = j.loc[idx]

print("\nMaximal relative error |ΔBz|/dBzBz1 found at Id =", idx)
print("z =", row["z_1"], " (x,y,z) = (", row["x_1"], ",", row["y_1"], ",", row["z_1"], ")")
print("Bz_1 =", row["Bz_1"])
print("Bz_2 =", row["Bz_2"])
print("ΔBz/Bz_1  =", row["dBzBz1"])
print("|ΔBz/Bz_1|=", abs(row["dBzBz1"]))

# Alle zugehörigen Feldkomponenten:
print(row[["x_1","y_1","z_1","Bx_1","By_1","Bz_1","Bx_2","By_2","Bz_2","dBz"]])
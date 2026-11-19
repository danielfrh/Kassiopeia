#!/usr/bin/env python3

# Study field points of z-component of electric field (E_z) on lines as calculated and
# stored by Application FieldCalculator_Electric with relative w.r.t. first input 

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

# Definition of relative error: df1 - df2) / df1
j["dEzRel"] = ((j["Ez_1"] - j["Ez_2"])/j["Ez_1"]).abs()

# 2) Plot: z auf x-Achse, Differenz auf y-Achse
# robust: nach z sortieren (falls Id nicht monoton in z ist)
plot_df = j.sort_values("z_1")

plt.figure(figsize=(7,4))
plt.plot(plot_df["z_1"], plot_df["dEzrel"], lw=1)
plt.xlabel("z")
plt.ylabel("|ΔEz|/Ez_1 = (Ez_1 - Ez_2)/Ez_1")
plt.title("Relative Error ΔEzRel against z-coordinate")
plt.grid(True)
plt.tight_layout()
plt.show()

# 3) Maximal relative error
# meist sinnvoll: maximale *Betrags*-Differenz
idx = j["dEzRel"].abs().idxmax()        # Id (Index)
row = j.loc[idx]

print("\nMaximal relative error |ΔEz|/dE_1 found at Id =", idx)
print("z =", row["z_1"], " (x,y,z) = (", row["x_1"], ",", row["y_1"], ",", row["z_1"], ")")
print("Ez_1 =", row["Ez_1"])
print("Ez_2 =", row["Ez_2"])
print("|ΔEz/Ez_1|=", abs(row["dEzRel"]))

# Alle zugehörigen Feldkomponenten:
print(row[["x_1","y_1","z_1","Bx_1","By_1","Bz_1","Bx_2","By_2","Bz_2","dBz"]])
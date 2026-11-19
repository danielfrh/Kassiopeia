#!/usr/bin/env python3
# Study field points of magnitude of electric field (E_mag) on lines as calculated and
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
j["dEmagRel"]=((j["Emag_1"]-j["Emag_2"] )/j["Emag_1"]).abs()

# 2) Plot: z auf x-Achse, Differenz auf y-Achse
# robust: nach z sortieren (falls Id nicht monoton in z ist)
plot_df = j.sort_values("z_1")

plt.figure(figsize=(7,4))
plt.plot(plot_df["z_1"], plot_df["dEmagRel"], lw=1)
plt.xlabel("z")
plt.ylabel("|ΔEmag|/Emag_1 = (Emag_1 - Emag_2)/Emag_1")
plt.title("Relative Error ΔEmagRel against z-coordinate")
plt.grid(True)
plt.tight_layout()
plt.show()

# 3) Maximale Differenz finden + zugehörige Spalten ausgeben
# meist sinnvoll: maximale *Betrags*-Differenz
idx = j["dEmagRel"].abs().idxmax()        # das ist die Id (Index)
row = j.loc[idx]

print("\nMaximal relative error |ΔEmag|/dE_1 found at Id =", idx)
print("z =", row["z_1"], " (x,y,z) = (", row["x_1"], ",", row["y_1"], ",", row["z_1"], ")")
print("Emag_1 =", row["Emag_1"])
print("Emag_2 =", row["Emag_2"])
print("|ΔEmag/Emag_1|=", abs(row["dEmagRel"]))

# Alle zugehörigen Feldkomponenten:
print(row[["x_1","y_1","z_1","Ex_1","Ey_1","Ez_1","Emag_1","Pot_1","Ex_2","Ey_2","Ez_2","Emag_2","Pot_2","dEmagRel"]])

#!/usr/bin/env python3

# Script zur Erstellung von Datei nur mit Feldlinienpunkten ausgehend von Inputdatei aus Berechnung von Feldlinien


# Das Extrahieren und Slicing der Spalten passiert mit df.iloc[:, :3] in einer einzigen Zeile.
# sep=None mit dem Python-Engine-Modul erkennt automatisch, ob die Spalten mit Leerzeichen, Tabs oder Kommas getrennt sind.
# Erweiterbarkeit fuer Berechnungen mit B

import pandas as pd
import sys

input_datei = sys.argv[1]
output_datei = sys.argv[2]

if len(sys.argv) != 3:
    print("Aufruf: python3 script.py inputdatei.csv outputdatei.csv")
    sys.exit(1)
for eachArg in sys.argv:
        print(eachArg)

try:
  # 1. Erste Zeile auslesen, um die Anzahl der Zeilen zu kennen (falls benötigt)
  with open(input_datei, "r", encoding="utf-8") as f:
    erste_zeile = f.readline().strip()

  # 2. Die Daten einlesen
  # skiprows=1 überspringt die Zeile mit der Zeilenanzahl
  # sep=None lässt pandas das Trennzeichen (Leerzeichen/Tabs) automatisch erkennen
  df = pd.read_csv(input_datei, skiprows=1, header=None, sep=None, engine="python")

  # 3. Nur die ersten drei Spalten behalten (Index 0, 1 und 2)
  df_neu = df.iloc[:, :3]

  # 4. In die neue Datei schreiben
  # Wir öffnen die Zieldatei, schreiben zuerst die alte erste Zeile rein
  # und hängen dann den pandas-DataFrame an.
  with open(output_datei, "w", encoding="utf-8") as f:
    f.write(erste_zeile + "\n")
    # DataFrame ohne Spaltennamen und Index als Text (mit Tabs getrennt) speichern
    df_neu.to_csv(f, sep="\t", header=False, index=False)

  print(
      f"Erfolgreich mit pandas! Gespeichert unter '{output_datei}'."
  )

except FileNotFoundError:
  print(f"Fehler: Die Datei '{input_datei}' wurde nicht gefunden.")

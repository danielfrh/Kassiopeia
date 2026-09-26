import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

# 1. CSV-Datei einlesen
# Da die erste Zeile ('4' als Anzahl der Punkte) übersprungen werden muss, nutzen wir skiprows=1.
# Da die Werte durch Leerzeichen getrennt sind, nutzen wir delim_whitespace=True.
df = pd.read_csv("test.csv", skiprows=1, header=None, delim_whitespace=True)

# 2. Spalten benennen (X, Y, Z und Magnetfeldwert)
df.columns = ["x", "y", "z", "magnetfeld"]

# 3. DataFrame in ein Pivot-Format bringen (X und Y als Achsen, Magnetfeld als Werte)
pivot_df = df.pivot(index="y", columns="x", values="magnetfeld")

# 4. Heatmap mit Seaborn erstellen
plt.figure(figsize=(6, 5))
sns.heatmap(
    pivot_df, annot=True, cmap="viridis", cbar_kws={"label": "Magnetfeld"}
)

# 5. Diagramm anpassen und anzeigen
plt.title("Magnetfeld Heatmap nach kartesischen Koordinaten")
plt.xlabel("X-Koordinate")
plt.ylabel("Y-Koordinate")
plt.tight_layout()

# Optional: Als Bild speichern
plt.savefig("magnetfeld_heatmap.png", dpi=300)
plt.show()

Erklärung der Schritte:
Einlesen mit Pandas (pd.read_csv):
skiprows=1 ignoriert die erste Zeile der Datei (die 4), da diese lediglich die Anzahl der Messpunkte angibt.
delim_whitespace=True sorgt dafür, dass die durch Leerzeichen getrennten Spalten korrekt erkannt werden.
Spaltenzuweisung: Die Spalten werden mit aussagekräftigen Namen versehen (x, y, z, magnetfeld).
Pivotieren (df.pivot): Seaborn-Heatmaps benötigen eine Matrix-Struktur, bei der die Zeilen und Spalten den Koordinaten
und die Zelleninhalte den Messwerten entsprechen.
Visualisierung (sns.heatmap):
annot=True zeigt die konkreten Magnetfeldwerte direkt in den Feldern an.
cmap='viridis' sorgt für eine moderne, leicht ablesbare Farbpalette.
Falls du noch weitere Koordinaten oder eine höhere Auflösung hast, skaliert dieses Skript automatisch mit!
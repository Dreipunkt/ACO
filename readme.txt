## Verwendung ##

Usage: ACO.exe Path_to_datafile Startnode Endnode Number_of_iterations

Path_to_datafile - Muss wie für das jeweilige OS üblich angegeben werden. Relative und absolute Pfade möglich. (nicht auf Dateiendung vergessen!)
Zum Format der Datei: Erste Zeile enthält nur die Anzahl der Knoten. Ab der zweiten Zeile wird die Adjazenzmatrix angegeben. Die Einträge werden mit Leerzeichen getrennt. Falls keine Verbindung zwischen zwei Knoten existiert ist dies mit "-1" anzugeben. "0" ist als Eintrag nicht erlaubt. Graphen ohne Kantengewichtung sind allerdings auch möglich, indem alle Verbindungen als "1" angegeben werden. Als Beispiel siehe "test.txt".

Startnode - Startknoten der Wegfindung (ACHTUNG: Zählung beginnt mit 0)

Endnode - Endknoten der Wegfindung (ACHTUNG: Zählung beginnt mit 0)

Number_of_iterations - Anzahl der Iterationen, um Approximation des idealen Weges zu verbessern. (zwischen 1 bis ca. 1000)


## Allgemeines ##

Konstanten wie alpha, beta, der Verfallfaktor von Spuren, ... sind derzeit noch im Sourcecode festgelegt.

"#define DEBUG_MODE false" kann auf "true" gesetzt werden, für ein ausführliches Debugging.



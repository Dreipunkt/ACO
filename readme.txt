## Verwendung ##

Usage: ACO.exe Path_to_datafile Startnode Endnode Number_of_iterations Debug_mode

Path_to_datafile - Muss wie für das jeweilige OS üblich angegeben werden. Relative und absolute Pfade möglich. (nicht auf Dateiendung vergessen!)
Zum Format der Datei: Erste Zeile enthält nur die Anzahl der Knoten. Ab der zweiten Zeile wird die Adjazenzmatrix angegeben. Die Einträge werden mit Leerzeichen getrennt. Falls keine Verbindung zwischen zwei Knoten existiert ist dies mit "-1" anzugeben. "0" ist als Eintrag nicht erlaubt. Graphen ohne Kantengewichtung sind allerdings auch möglich, indem alle Verbindungen als "1" angegeben werden. Als Beispiel siehe ACO_Examples.

Startnode - Startknoten der Wegfindung (ACHTUNG: Zählung beginnt mit 0, d.h. wenn im File 5 Knoten eingegeben wurden, ist der letzte Knoten 4)

Endnode - Endknoten der Wegfindung (ACHTUNG: Zählung beginnt mit 0, d.h. wenn im File 5 Knoten eingegeben wurden, ist der letzte Knoten 4)

Number_of_iterations - Anzahl der Iterationen, um Approximation des idealen Weges zu verbessern. (zwischen 1 bis ca. 1000)

Debug_mode - Ausführlichere Ausgabe ('0' deaktivieren, '1' aktivieren)

## Allgemeines ##

Konstanten wie alpha, beta, der Verfallfaktor von Spuren, usw. wurden (mit sinnvollen Werten) im Sourcecode festgelegt.

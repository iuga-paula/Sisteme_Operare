# Sisteme de Operare
## _Cabinet Medical_

Proiect individual - programarea unei cozi de pacienți la medici. Se creează  un număr dat de thread-uri pacienți care
vor aștepta pentru eliberarea unor resurse reprezentând doctorii (pot fi niște structuri
iar consultația  consistă în  blocarea acelei structuri și afișarea id-ului doctorului). Clienții
vor ocupa resursa doctor pentru o perioadă random care să nu depășească o limită de timp.
Fiecare pacient va fi generat la un interval aleator pentru o perioadă data de timp. După
consultație, pacientulului, i va afișa timpul de așteptare și timpul consultației.

## Tehnologii

* C
* Threaduri
* Mutex

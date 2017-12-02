Per modificare l'unit� di misura del valore che si vuole visualizzare, � necessario scrivere al termine della stringa un underscore "_" seguito dall'unit� di misura desiderata; per concatenare pi� comandi, utilizzare il punto e virgola ";".

Se non viene specificata alcuna unit� di misura, o viene inserit� una metrica errata, verr� automaticamente utilizzata quella di default, ossia metri per le distanze, m/s per la velocit� e gradi per gli angoli.

Metriche supportate:
- Velocit�: cm/s, m/s (default), km/h
- Distanze: mm, cm, m (default)
- Angoli: gradi (default), rad

Esempio di comando:

Telemetry VehicleSimData.ChassisSimData.Speed;VehicleSimData.ChassisSimData.Speed_km/h;VehicleSimData.ChassisSimData.Speed_cm/s
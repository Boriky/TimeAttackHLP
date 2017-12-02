Per modificare l'unità di misura del valore che si vuole visualizzare, è necessario scrivere al termine della stringa un underscore "_" seguito dall'unità di misura desiderata; per concatenare più comandi, utilizzare il punto e virgola ";".

Se non viene specificata alcuna unità di misura, o viene inserità una metrica errata, verrà automaticamente utilizzata quella di default, ossia metri per le distanze, m/s per la velocità e gradi per gli angoli.

Metriche supportate:
- Velocità: cm/s, m/s (default), km/h
- Distanze: mm, cm, m (default)
- Angoli: gradi (default), rad

Esempio di comando:

Telemetry VehicleSimData.ChassisSimData.Speed;VehicleSimData.ChassisSimData.Speed_km/h;VehicleSimData.ChassisSimData.Speed_cm/s
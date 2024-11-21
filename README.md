# Progetto Client/Server in linguaggio C
Ingestion CSV tramite bulk DDL COPY su REDSHIFT CLUSTER

---------------------------------------------
STEP FUNCTION - sfNplRedshiftDataIngestion
---------------------------------------------
1) <strong>Evento Init JSON per la Step Function</strong>: 
</br>
	{
    "SEARCH_FILTER": "migrazione20dicembre/fase4_DWHdaRADaBTV"
  }

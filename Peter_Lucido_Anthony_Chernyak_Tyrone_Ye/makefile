main: Servers Play Clean

Servers: DRMserver.c Saveserver.c
	gcc DRMserver.c -o Verify
	gcc Saveserver.c -o ToSave

Play: DRM.c Engine
	gcc DRM.c

Engine: Engine.c Headers
	gcc Engine.c -o execute
Headers: headers.h
	gcc headers.h
Clean:
	rm headers.h.gch

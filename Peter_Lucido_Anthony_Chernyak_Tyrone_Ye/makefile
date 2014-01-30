main: Servers Play Clean

Servers: DRMserver.c SaveServer.c
	gcc DRMserver.c - Verify
	gcc SaveServer.c - ToSave

Play: DRM.c Engine
	gcc DRM.c

Engine: Engine.c Headers
	gcc Engine.c -o execute
Headers: headers.h
	gcc headers.h
Clean:
	rm headers.h.gch
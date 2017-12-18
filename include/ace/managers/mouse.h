#ifndef GUARD_ACE_MANAGER_MOUSE_H
#define GUARD_ACE_MANAGER_MOUSE_H

#include <ace/types.h>
#include <ace/macros.h>

// Mouse button identifiers
#define MOUSE_LMB 0
#define MOUSE_RMB 1
#define MOUSE_MMB 2

// Button state defines
#define MOUSE_NACTIVE 0
#define MOUSE_USED 1
#define MOUSE_ACTIVE 2

// Mouse ports
// #define MOUSE_PORT_0 0 // Unused mouse port for alignment
#define MOUSE_PORT_1 1
#define MOUSE_PORT_2 2

/* Types */
typedef struct _tMouse {
	UWORD uwX;
	UWORD uwY;
	UBYTE pButtonStates[3];
#ifdef AMIGA
	UBYTE ubPrevHwX;
	UBYTE ubPrevHwY;
#endif
} tMouse;

typedef struct _tMouseManager {
	UBYTE ubPortFlags;
	tMouse pMouses[3]; ///< Zero is pad, faster than subtracting from port code.
	UWORD uwLoX; ///< Minimum cursor's X position
	UWORD uwLoY; ///< Minimum cursor's Y position
	UWORD uwHiX; ///< Maximum cursor's X position
	UWORD uwHiY; ///< Maximum cursor's Y position
#ifdef AMIGA
	UWORD uwPrevPotGo; ///< Previous control port config.
#endif // AMIGA
} tMouseManager;

/* Globals */
extern tMouseManager g_sMouseManager;

/* Functions */

/**
 * Initializes mouse management for given ports.
 * @param ubPortFlags: Ports in which mouse should be processed.
 *        OR combination of MOUSE_PORT_* defines.
 * @see mouseDestroy()
 * @see mouseProcess()
 */
void mouseCreate(
	IN UBYTE ubPortFlags
);

/**
 * Cleans up after mouse maanger.
 * @see mouseCreate()
 */
void mouseDestroy(void);

/**
 * Processes mouse manager, updating mice's position and button states.
 * Should be called once per frame.
 */
void mouseProcess(void);

/**
 * Set on-screen constraints for cursor.
 * @param uwLoX Minimum cursor X position.
 * @param uwLoX Minimum cursor Y position.
 * @param uwHiY Maximum cursor X position.
 * @param uwHiY Maximum cursor Y position.
 */
void mouseSetBounds(
	IN UWORD uwLoX,
	IN UWORD uwLoY,
	IN UWORD uwHiX,
	IN UWORD uwHiY
);

/**
 * Returns given mouse's current X position.
 * @param ubMousePort: Mouse to be polled. Use one of MOUSE_PORT_* values.
 * @return Mouse's current X position relative to top-left screen pos.
 */
static inline UWORD mouseGetX(
	IN UBYTE ubMousePort
) {
	return g_sMouseManager.pMouses[ubMousePort].uwX;
}

/**
 * Returns given mouse's current Y position.
 * @param ubMousePort: Mouse to be polled. Use one of MOUSE_PORT_* values.
 * @return Mouse's current X position relative to top-left screen pos.
 */
static inline UWORD mouseGetY(
	IN UBYTE ubMousePort
) {
	return g_sMouseManager.pMouses[ubMousePort].uwY;
}

/**
 * Sets given mouse's button to desired state.
 * @param ubMousePort: Mouse to be set.
 * @param ubMouseCode: Mouse button, which state should be changed
 *        (MOUSE_LMB, MOUSE_RMB or MOUSE_MMB).
 * @param ubMouseState: New button state
 *        (MOUSE_NACTIVE, MOUSE_USED, MOUSE_ACTIVE).
 */
static inline void mouseSetButton(
	IN UBYTE ubMousePort,
	IN UBYTE ubMouseCode,
	IN UBYTE ubMouseState
) {
	g_sMouseManager.pMouses[ubMousePort].pButtonStates[ubMouseCode] = ubMouseState;
}

/**
 * Returns given mouse's button state.
 * @param ubMousePort: Mouse to be polled.
 * @param ubMouseCode: Button to be polled (MOUSE_LMB, MOUSE_RMB or MOUSE_MMB).
 * @return Current mouse sate: (MOUSE_NACTIVE, MOUSE_USED, MOUSE_ACTIVE).
 */
static inline UBYTE mouseCheck(
	IN UBYTE ubMousePort,
	IN UBYTE ubMouseCode
) {
	UBYTE ubBtn = g_sMouseManager.pMouses[ubMousePort].pButtonStates[ubMouseCode];
	return ubBtn != MOUSE_NACTIVE;
}

/**
 * Returns whether given button was recently pressed.
 * If button was polled as ACTIVE, function returns 1 and sets button as USED.
 * @param ubMousePort: Mouse to be polled.
 * @param ubMouseCode: Button to be polled (MOUSE_LMB, MOUSE_RMB or MOUSE_MMB).
 * @return 1 if button was recently pressed, otherwise 0.
 */
static inline UBYTE mouseUse(
	IN UBYTE ubMousePort,
	IN UBYTE ubMouseCode
) {
	tMouse *pMouse = &g_sMouseManager.pMouses[ubMousePort];
	if(pMouse->pButtonStates[ubMouseCode] == MOUSE_ACTIVE) {
		pMouse->pButtonStates[ubMouseCode] = MOUSE_USED;
		return 1;
	}
	return 0;
}

/**
 * Checks if given mouse has position contained within given rectangle.
 * @param ubMousePort: Mouse to be polled.
 * @param sRect: Rectangle to be checked.
 * @return 1 if mouse position is within given rectangle, otherwise 0.
 */
static inline UBYTE mouseInRect(
	IN UBYTE ubMousePort,
	IN tUwRect sRect
) {
	UWORD uwMouseX = g_sMouseManager.pMouses[ubMousePort].uwX;
	UWORD uwMouseY = g_sMouseManager.pMouses[ubMousePort].uwY;
	return (
		(sRect.uwX <= uwMouseX) && (uwMouseX < sRect.uwX + sRect.uwWidth) &&
		(sRect.uwY <= uwMouseY) && (uwMouseY < sRect.uwY + sRect.uwHeight)
	);
}

/**
 *  Sets mouse position to given absolute position.
 * This function takes into account bounds specified by mouseSetBounds().
 * @param ubMousePort: Mouse of which position should be changed.
 * @param uwNewX: new X position.
 * @param uwNewY: new Y position.
 * @see mouseSetBounds()
 * @see mouseMoveBy()
 */
static inline void mouseSetPosition(
	IN UBYTE ubMousePort,
	IN UWORD uwNewX,
	IN UWORD uwNewY
) {
	g_sMouseManager.pMouses[ubMousePort].uwX = CLAMP(
		uwNewX, g_sMouseManager.uwLoX, g_sMouseManager.uwHiX
	);
	g_sMouseManager.pMouses[ubMousePort].uwY = CLAMP(
		uwNewY, g_sMouseManager.uwLoY, g_sMouseManager.uwHiY
	);
}

/**
 * Moves mouse pointer from current position by relative offsets.
 * This function takes into account bounds specified by mouseSetBounds().
 * @param ubMousePort: Mouse of which position should be changed.
 * @param wDx: Positive value moves mouse right, negative moves left.
 * @param wDy: Positive value moves mouse down, negative moves left.
 * @see mouseSetBounds()
 * @see mouseSetPosition()
 */
static inline void mouseMoveBy(
	IN UBYTE ubMousePort,
	IN WORD wDx,
	IN WORD wDy
) {
	g_sMouseManager.pMouses[ubMousePort].uwX = CLAMP(
		g_sMouseManager.pMouses[ubMousePort].uwX + wDx,
		g_sMouseManager.uwLoX, g_sMouseManager.uwHiX
	);
	g_sMouseManager.pMouses[ubMousePort].uwY = CLAMP(
		g_sMouseManager.pMouses[ubMousePort].uwY + wDy,
		g_sMouseManager.uwLoY, g_sMouseManager.uwHiY
	);
}

#endif

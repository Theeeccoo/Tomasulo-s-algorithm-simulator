  /***** =========== ****/
 /***** DEFINITIONS ****/
/***** =========== ****/

/**
 * @file Cmulator/reservation_station.h
 *
 * @brief Reservation Station management 
 */
#ifndef RESERVATION_STATION_H_
#define RESERVATION_STATION_H_

#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"
#include "reorder_buffer.h"

#define BOOL	 	int
#define TRUE	 	1
#define FALSE	 	0 
#define SIZE_STR	30

#ifndef RESERVATION_STATION_LINE_

		/**
		 * @name Information about reservation station
		 */
		/**@{*/
		#define	NOT_BUSY	FALSE	/**< Reservation station is not with instruction */
		#define BUSY		TRUE	/**< Reservation station is with instruction */
		/**@}*/
		
        /**
		 * @name Instruction types
		 */
		/**@{*/
		#define LOAD		0	/**< Operating unit load */
		#define ADD     	1	/**< Operating unit add */
		#define MULT	    2	/**< Operating unit mult */
		/**@}*/

		/**
		 * @brief Line of a Reservation Station
		 */
		typedef struct Reservation_Station_Line {
			/**
			 * @name Reservation station status
			 */
			/**@{*/
			BOOL reservation_busy;	/**< Reservation station is with instruction? */
			/**@}*/
			
			/**
			 * @name Reservation station data
			 */
			/**@{*/
			char* name;     /**< Name of reservation station*/
			int type;      /**< Type of operating unit*/
			char* instruction_op;		/**< Name of instruction */
			char* value_register_read_Vj;	/**< Read register value - Vj */
			char* value_register_read_Vk;	/**< Read register value - Vk */
			int information_dependency_Qj;    /**< Read register dependency information - Qj*/
			int information_dependency_Qk;    /**< Read register dependency information - Qk*/
			int position_destination_rb;  /**< Which position in reorder buffer the instruction result should go */
			char* memory_address;   /**< Memory address*/
			/**@}*/
		} Reservation_Station_Line;

#endif /* RESERVATION_STATION_LINE_ */

	/**
	 * @name Reservation station line controller
	 */
	/**@{*/
	#define MAX_LINES_RS	 12	/* Maximum ammount of possible lines in reservation station*/
	/**@}*/

	typedef struct Reservation_Station {
		Reservation_Station_Line line[MAX_LINES_RS];	/**< Lines of our reservation station */
	} Reservation_Station;

#endif /* RESERVATION_STATION_H_ */

/** 
 * @name Reservation station operations
 */
/**@{*/
extern Reservation_Station* reservationStationInitializer();
extern int insertInstructionRS(Instruction *instruction, Reservation_Station *reservationStation, Reorder_Buffer *rb);
extern int findLineRSAccordingPositionRB (Reservation_Station *rs, int positionRB);
extern int dontDoWrite(char* nameInstruction);
extern int warDependencyIdentifier(char*, int, Reorder_Buffer*);
extern void printReservationStation(Reservation_Station*);
extern void clearLineRS (Reservation_Station *reservationStation, int positionRS);
//extern Reservation_Station* freesReservationStation(Reservation_Station*);
/**@}*/

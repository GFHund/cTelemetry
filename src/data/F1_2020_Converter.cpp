#include "F1_2020_Converter.h"

#include "Exceptions/SQLErrorException.h"
#include "Exceptions/FileNotFoundException.h"
#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <math.h>
#include <chrono>
#include <float.h>

#pragma pack(push)
#pragma pack(1)
struct PacketHeader
{
    uint16_t    m_packetFormat;             // 2020
    uint8_t     m_gameMajorVersion;         // Game major version - "X.00"
    uint8_t     m_gameMinorVersion;         // Game minor version - "1.XX"
    uint8_t     m_packetVersion;            // Version of this packet type, all start from 1
    uint8_t     m_packetId;                 // Identifier for the packet type, see below
    char    m_sessionUID[8];               // Unique identifier for the session
    char     m_sessionTime[4];              // Session timestamp
    char m_frameIdentifier[4];          // Identifier for the frame the data was retrieved on
    uint8_t     m_playerCarIndex;           // Index of player's car in the array

   // ADDED IN BETA 2:
    uint8_t     m_secondaryPlayerCarIndex;  // Index of secondary player's car in the array (splitscreen)
                                          // 255 if no second player
};

struct ParticipantData
{
    uint8_t      m_aiControlled;           // Whether the vehicle is AI (1) or Human (0) controlled
    uint8_t      m_driverId;               // Driver id - see appendix
    uint8_t      m_teamId;                 // Team id - see appendix
    uint8_t      m_raceNumber;             // Race number of the car
    uint8_t      m_nationality;            // Nationality of the driver
    char       m_name[48];               // Name of participant in UTF-8 format � null terminated
                                         // Will be truncated with � (U+2026) if too long
    uint8_t      m_yourTelemetry;          // The player's UDP setting, 0 = restricted, 1 = public
};

struct PacketParticipantsData
{
    PacketHeader    m_header;           // Header

    uint8_t           m_numActiveCars;	// Number of active cars in the data � should match number of
                                        // cars on HUD
    ParticipantData m_participants[22];
};

struct LapData
{
    float    m_lastLapTime;               // Last lap time in seconds
    float    m_currentLapTime;            // Current time around the lap in seconds

    //UPDATED in Beta 3:
    uint16_t   m_sector1TimeInMS;           // Sector 1 time in milliseconds
    uint16_t   m_sector2TimeInMS;           // Sector 2 time in milliseconds
    float    m_bestLapTime;               // Best lap time of the session in seconds
    uint8_t    m_bestLapNum;                // Lap number best time achieved on
    uint16_t   m_bestLapSector1TimeInMS;    // Sector 1 time of best lap in the session in milliseconds
    uint16_t   m_bestLapSector2TimeInMS;    // Sector 2 time of best lap in the session in milliseconds
    uint16_t   m_bestLapSector3TimeInMS;    // Sector 3 time of best lap in the session in milliseconds
    uint16_t   m_bestOverallSector1TimeInMS;// Best overall sector 1 time of the session in milliseconds
    uint8_t    m_bestOverallSector1LapNum;  // Lap number best overall sector 1 time achieved on
    uint16_t   m_bestOverallSector2TimeInMS;// Best overall sector 2 time of the session in milliseconds
    uint8_t    m_bestOverallSector2LapNum;  // Lap number best overall sector 2 time achieved on
    uint16_t   m_bestOverallSector3TimeInMS;// Best overall sector 3 time of the session in milliseconds
    uint8_t    m_bestOverallSector3LapNum;  // Lap number best overall sector 3 time achieved on


    float    m_lapDistance;               // Distance vehicle is around current lap in metres � could
                                          // be negative if line hasn�t been crossed yet
    float    m_totalDistance;             // Total distance travelled in session in metres � could
                                          // be negative if line hasn�t been crossed yet
    float    m_safetyCarDelta;            // Delta in seconds for safety car
    uint8_t    m_carPosition;               // Car race position
    uint8_t    m_currentLapNum;             // Current lap number
    uint8_t    m_pitStatus;                 // 0 = none, 1 = pitting, 2 = in pit area
    uint8_t    m_sector;                    // 0 = sector1, 1 = sector2, 2 = sector3
    uint8_t    m_currentLapInvalid;         // Current lap invalid - 0 = valid, 1 = invalid
    uint8_t    m_penalties;                 // Accumulated time penalties in seconds to be added
    uint8_t    m_gridPosition;              // Grid position the vehicle started the race in
    uint8_t    m_driverStatus;              // Status of driver - 0 = in garage, 1 = flying lap
                                          // 2 = in lap, 3 = out lap, 4 = on track
    uint8_t    m_resultStatus;              // Result status - 0 = invalid, 1 = inactive, 2 = active
                                          // 3 = finished, 4 = disqualified, 5 = not classified
                                          // 6 = retired
};


struct PacketLapData
{
    PacketHeader    m_header;             // Header
    LapData         m_lapData[22];        // Lap data for all cars on track
};


struct CarTelemetryData
{
    uint16_t    m_speed;                         // Speed of car in kilometres per hour
    float     m_throttle;                      // Amount of throttle applied (0.0 to 1.0)
    float     m_steer;                         // Steering (-1.0 (full lock left) to 1.0 (full lock right))
    float     m_brake;                         // Amount of brake applied (0.0 to 1.0)
    uint8_t     m_clutch;                        // Amount of clutch applied (0 to 100)
    int8_t      m_gear;                          // Gear selected (1-8, N=0, R=-1)
    uint16_t    m_engineRPM;                     // Engine RPM
    uint8_t     m_drs;                           // 0 = off, 1 = on
    uint8_t     m_revLightsPercent;              // Rev lights indicator (percentage)
    uint16_t    m_brakesTemperature[4];          // Brakes temperature (celsius)
    uint8_t     m_tyresSurfaceTemperature[4];    // Tyres surface temperature (celsius)
    uint8_t     m_tyresInnerTemperature[4];      // Tyres inner temperature (celsius)
    uint16_t    m_engineTemperature;             // Engine temperature (celsius)
    float     m_tyresPressure[4];              // Tyres pressure (PSI)
    uint8_t     m_surfaceType[4];                // Driving surface, see appendices
};

struct PacketCarTelemetryData
{
    PacketHeader    	m_header;	       // Header

    CarTelemetryData    m_carTelemetryData[22];

    unsigned int              m_buttonStatus;        // Bit flags specifying which buttons are being pressed
                                               // currently - see appendices

    // Added in Beta 3:
    uint8_t               m_mfdPanelIndex;       // Index of MFD panel open - 255 = MFD closed
                                               // Single player, race � 0 = Car setup, 1 = Pits
                                               // 2 = Damage, 3 =  Engine, 4 = Temperatures
                                               // May vary depending on game mode
    uint8_t               m_mfdPanelIndexSecondaryPlayer;   // See above
    int8_t                m_suggestedGear;       // Suggested gear for the player (1-8)
                                               // 0 if no gear suggested
};

struct CarMotionData
{
    float         m_worldPositionX;           // World space X position
    float         m_worldPositionY;           // World space Y position
    float         m_worldPositionZ;           // World space Z position
    float         m_worldVelocityX;           // Velocity in world space X
    float         m_worldVelocityY;           // Velocity in world space Y
    float         m_worldVelocityZ;           // Velocity in world space Z
    int16_t         m_worldForwardDirX;         // World space forward X direction (normalised)
    int16_t         m_worldForwardDirY;         // World space forward Y direction (normalised)
    int16_t         m_worldForwardDirZ;         // World space forward Z direction (normalised)
    int16_t         m_worldRightDirX;           // World space right X direction (normalised)
    int16_t         m_worldRightDirY;           // World space right Y direction (normalised)
    int16_t         m_worldRightDirZ;           // World space right Z direction (normalised)
    float         m_gForceLateral;            // Lateral G-Force component
    float         m_gForceLongitudinal;       // Longitudinal G-Force component
    float         m_gForceVertical;           // Vertical G-Force component
    float         m_yaw;                      // Yaw angle in radians
    float         m_pitch;                    // Pitch angle in radians
    float         m_roll;                     // Roll angle in radians
};

struct PacketMotionData
{
    PacketHeader    m_header;               	// Header

    CarMotionData   m_carMotionData[22];    	// Data for all cars on track

    // Extra player car ONLY data
    float         m_suspensionPosition[4];      // Note: All wheel arrays have the following order:
    float         m_suspensionVelocity[4];      // RL, RR, FL, FR
    float         m_suspensionAcceleration[4];	// RL, RR, FL, FR
    float         m_wheelSpeed[4];           	// Speed of each wheel
    float         m_wheelSlip[4];               // Slip ratio for each wheel
    float         m_localVelocityX;         	// Velocity in local space
    float         m_localVelocityY;         	// Velocity in local space
    float         m_localVelocityZ;         	// Velocity in local space
    float         m_angularVelocityX;		    // Angular velocity x-component
    float         m_angularVelocityY;           // Angular velocity y-component
    float         m_angularVelocityZ;           // Angular velocity z-component
    float         m_angularAccelerationX;       // Angular velocity x-component
    float         m_angularAccelerationY;	    // Angular velocity y-component
    float         m_angularAccelerationZ;       // Angular velocity z-component
    float         m_frontWheelsAngle;           // Current front wheels angle in radians
};
#pragma pack(pop)
//--------------------------------------
bool F1_2020_Converter::isFile(sqlite3* db){
    std::string sql = std::string("SELECT packetFormat FROM packets LIMIT 1;");
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db,sql.c_str(),sql.size(),&stmt,NULL) != SQLITE_OK){
        throw SQLErrorException(sqlite3_errmsg(db),sql);
    }
    int ret_code = 0;
    std::vector<std::string> tables;
    bool is2020F1 = false;
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
        int packetFormat = sqlite3_column_int(stmt,0);
        if(packetFormat == 2020){
            is2020F1 = true;
        }
        //tables.push_back(  );
    }
    sqlite3_finalize(stmt);
    return is2020F1;
}
//--------------------------------------
sqlite3* F1_2020_Converter::convert(std::string path,sqlite3* db){
    path.replace(path.size()-7,7,"intern.sqlite3");
    sqlite3* inMemoryDb;
    //int rc = sqlite3_open(path.c_str(),&convertedDb);
	int rc = sqlite3_open(":memory:",&inMemoryDb);
    if(rc){
        sqlite3_close(db);
        throw FileNotFoundException(path);
    }
    try{
		
        this->createFileInfoTable(inMemoryDb);
		this->insertFileInfoTable(inMemoryDb);

        this->createDriversTable(db, inMemoryDb);
		this->insertDriversTable(db, inMemoryDb);

        this->createLapTable(db, inMemoryDb);
		this->insertLapTable(db, inMemoryDb);

        this->createLegendTable(inMemoryDb);
		this->insertLegendTable(inMemoryDb);
        this->createDataTables(db, inMemoryDb);

		sqlite3* convertedDb;
		int rc = sqlite3_open(path.c_str(),&convertedDb);
		if(rc){
			sqlite3_close(convertedDb);
			sqlite3_close(inMemoryDb);
			throw FileNotFoundException(path);
		}
		
		sqlite3_backup *pBackup;
		pBackup = sqlite3_backup_init(convertedDb, "main", inMemoryDb, "main");
		if( pBackup ){
			(void)sqlite3_backup_step(pBackup, -1);
			(void)sqlite3_backup_finish(pBackup);
			sqlite3_close(inMemoryDb);
		}
		else{
			rc = sqlite3_errcode(convertedDb);
			if(rc != SQLITE_DONE){
				throw SQLErrorException(sqlite3_errmsg(convertedDb));
			}
		}
		
		
        return convertedDb;
    }catch(SQLErrorException e){
        throw e;
    }
    
}
//--------------------------------------
//--------------------------------------

//--------------------------------------
void F1_2020_Converter::insertDriversTable(sqlite3* f1Db,sqlite3* convertedDb){
	std::cout << "=================== Insert Driver Table ======================" << std::endl;
    sqlite3_stmt* stmt;
	int ret_code;

    std::string insertSql = "INSERT INTO driver(name,number) VALUES ";
    std::string selectSql = "SELECT packet FROM packets WHERE packetID = 4 ORDER BY frameIdentifier LIMIT 1";
    if(sqlite3_prepare_v2(f1Db,selectSql.c_str(),selectSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(f1Db),selectSql);
	}
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
    	PacketParticipantsData* parcipant = (PacketParticipantsData*)sqlite3_column_blob(stmt,0);

    	for(int i=0;i<22;i++){
    		int raceNumber = parcipant->m_participants[i].m_raceNumber;
    		std::string name = parcipant->m_participants[i].m_name;

    		if(name.empty()){
    			continue;
    		}
    		if(i != 0){
    			insertSql += ",";
    		}
    		insertSql += "(\"";
    		insertSql += name;
    		insertSql += "\",";
    		insertSql += std::to_string(raceNumber);
    		insertSql += ")";
    	}
    }
    sqlite3_finalize(stmt);

    if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
    ret_code = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
}
//--------------------------------------
void F1_2020_Converter::insertLapTable(sqlite3* f1Db,sqlite3* convertedDb){
	std::cout << "=================== Insert Lap Table ======================" << std::endl;
	sqlite3_stmt* stmt;
	int ret_code;
	
	std::string insertSql = "INSERT INTO lap(driver,lap_number,lap_time) VALUES ";
	std::string selectSql = "SELECT packet FROM packets WHERE packetID = 2 ORDER BY frameIdentifier";
	if(sqlite3_prepare_v2(f1Db,selectSql.c_str(),selectSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(f1Db),selectSql);
	}
	struct lapData{
		int driver;
		int lapNumber;
		float lapTime;
	};
	std::array<std::vector<lapData>,22> insertLapData;
	std::array<int,22> aCurrentLapNum;
	for(int i=0;i<22;i++){
		aCurrentLapNum[i] = -1;
		insertLapData[i] = std::vector<lapData>();
	}

	while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
		PacketLapData* lapData = (PacketLapData*)sqlite3_column_blob(stmt,0);

		for(int i=0;i<22;i++){
			int currentLap = lapData->m_lapData[i].m_currentLapNum;
			float currentLapTime = lapData->m_lapData[i].m_currentLapTime;
			
			if(currentLap == 0){//Lap 0 is not possible
				continue;
			}
			if(abs(currentLapTime) < FLT_MIN ){
				continue;
			}
			
			bool bHasLap = false;
			for(int j = 0; j < insertLapData[i].size();j++){
				if(insertLapData[i][j].lapNumber == currentLap){
					bHasLap = true;
					if(insertLapData[i][j].lapTime < currentLapTime){
						insertLapData[i][j].lapTime = currentLapTime;
					}
					break;
				}
			}
			
			if(!bHasLap){
				struct lapData l;
				l.driver = i;
				l.lapNumber = currentLap;
				l.lapTime = currentLapTime;
				insertLapData[i].push_back(l);
			}
			
		}
	}

	sqlite3_finalize(stmt);
	std::cout << "finished Collect Data" << std::endl;

	for(int i=0;i<22;i++){
		for(auto iter=insertLapData[i].begin();iter != insertLapData[i].end();iter++){
			if(isnan(iter->lapTime)){
				continue;
			}
			if((i == 0 && iter != insertLapData[i].begin()) || i != 0){
				insertSql += ",";
			}
			
			insertSql += "(";
			insertSql += std::to_string(i+1);
			insertSql += ",";
			insertSql += std::to_string(iter->lapNumber);
			insertSql += ",";
			insertSql += std::to_string(iter->lapTime);
			insertSql += ")\n";
		}
	}

	if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
}
//--------------------------------------

//--------------------------------------
void F1_2020_Converter::createDataTables(sqlite3* f1Db,sqlite3* convertedDb){
	createReferenceDataTable(f1Db, convertedDb);
	insertReferenceDataTable(f1Db, convertedDb);
	
	createFloatDataTable(f1Db, convertedDb);
	insertFloatDataTable(f1Db, convertedDb);

	createIntDataTable(f1Db, convertedDb);
	insertIntDataTable(f1Db, convertedDb);

	createVec3DataTable(f1Db, convertedDb);
	insertVec3DataTable(f1Db, convertedDb);
}
//--------------------------------------
void F1_2020_Converter::insertReferenceDataTable(sqlite3*f1Db,sqlite3* convertedDb){
	std::cout << "=================== Insert Reference Data Table ======================" << std::endl;
	sqlite3_stmt* stmt;
	int ret_code;

	std::string insertSql = "INSERT INTO reference_unit(lap_id, lap_distance, lap_time, session_time, frame_identifier,lap_invalid) VALUES ";
	std::string selectSql = "SELECT packet FROM packets WHERE packetID = 2 GROUP BY frameIdentifier ORDER BY frameIdentifier";
	//select statement
	if(sqlite3_prepare_v2(f1Db,selectSql.c_str(),selectSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(f1Db),selectSql);
	}
	bool bFirst = true;
	int rows = sqlite3_data_count(stmt); 
	std::cout << "ReferenceTable: Total Rows: " << rows << std::endl;
	int numData = 0;
	int iNumberInserts = 0;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
		PacketLapData* lapData = (PacketLapData*)sqlite3_column_blob(stmt,0);
		for(int i=0;i<22;i++){
			int* frameIdentifier = (int*)lapData->m_header.m_frameIdentifier;

			float lapDistance = lapData->m_lapData[i].m_lapDistance;
			float currentLapTime = lapData->m_lapData[i].m_currentLapTime;
			float *sessionTime = (float*)lapData->m_header.m_sessionTime;
			/*
			ofs << std::to_string(i) << std::endl;
			ofs << std::to_string(*frameIdentifier) << std::endl;
			ofs << std::to_string(lapDistance) << std::endl;
			ofs << std::to_string(currentLapTime) << std::endl;
			ofs << "------------------------------------------------" << std::endl;
			*/

			std::string selectConvertDb = "SELECT id FROM lap WHERE driver = ";
			selectConvertDb += std::to_string(i+1);
			selectConvertDb += " AND lap_number = ";
			selectConvertDb += std::to_string(lapData->m_lapData[i].m_currentLapNum);
			sqlite3_stmt* stmt2;
			
			if(sqlite3_prepare_v2(convertedDb,selectConvertDb.c_str(),selectConvertDb.size(),&stmt2,NULL) != SQLITE_OK){
				throw SQLErrorException(sqlite3_errmsg(convertedDb),selectConvertDb);
			}
			int ret_code2 = sqlite3_step(stmt2);
			
			char* sFrameIdentifier = lapData->m_header.m_frameIdentifier;
			int* iFrameIdentifier = (int*)sFrameIdentifier;
			if(ret_code2 == SQLITE_ROW){
				int id = sqlite3_column_int(stmt2,0);
				if(bFirst){
					bFirst = false;
				}else{
					insertSql += ",";
				}
				insertSql += "(";
				insertSql += std::to_string(id);
				insertSql += ",";
				insertSql += std::to_string(lapDistance);
				insertSql += ",";
				insertSql += std::to_string(currentLapTime);
				insertSql += ",";
				insertSql += std::to_string(*sessionTime);
				insertSql += ",";
				insertSql += std::to_string(*iFrameIdentifier);
				insertSql += ",";
				insertSql += std::to_string(lapData->m_lapData[i].m_currentLapInvalid);
				insertSql += ")\n";
				numData++;
				if(numData > 1000){
					std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					std::cout << "ReferenceTable: Select Statment" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" << std::endl;

					iNumberInserts++;
					std::cout << "ReferenceTable: " << rows << "/" << iNumberInserts << std::endl;

					sqlite3_stmt* stmt3;
					begin = std::chrono::steady_clock::now();
					if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt3,NULL) != SQLITE_OK){
						throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
					}
					int ret_code3 = sqlite3_step(stmt3);
					end = std::chrono::steady_clock::now();
					std::cout << "ReferenceTable: Insert Statment" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" << std::endl;
					sqlite3_finalize(stmt3);
					if(!(ret_code3 == SQLITE_DONE || ret_code3 == SQLITE_OK)){
						throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
					}
					insertSql = "INSERT INTO reference_unit(lap_id, lap_distance, lap_time, session_time, frame_identifier,lap_invalid) VALUES ";
					numData = 0;
					bFirst = true;
					begin = std::chrono::steady_clock::now();
				}
			}
			sqlite3_finalize(stmt2);
		}
		
	}
	sqlite3_finalize(stmt);

	
	

	if(numData > 0){
		if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
			std::string errorMsg = std::string(sqlite3_errmsg(convertedDb));
			throw SQLErrorException(errorMsg,insertSql);
		}
		ret_code = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		if(ret_code != SQLITE_DONE){
			throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
		}
	}

}

//--------------------------------------
void F1_2020_Converter::insertFloatDataTable(sqlite3* f1Db, sqlite3* convertedDb){
	std::cout << "=================== Insert Float Data Table ======================" << std::endl;
	sqlite3_stmt* stmt;
	int ret_code;

	std::string insertSql = "INSERT INTO float_data(reference_unit_id, property_id, float_val) VALUES ";
	std::string selectPacketSql = "SELECT packet FROM packets WHERE packetId = 6";
	if(sqlite3_prepare_v2(f1Db,selectPacketSql.c_str(),selectPacketSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(f1Db),selectPacketSql);
	}
	int rows = sqlite3_data_count(stmt) ;
	std::cout << "Float Data Table: Total Rows: " << rows << std::endl;
	int iNumberInserts = 0;
	bool bFirst = true;
	int numData = 0;

	std::string convertedSelect = "SELECT ref.id FROM reference_unit ref INNER JOIN lap l ON l.id = ref.lap_id WHERE ref.frame_identifier = @frameIdentifier AND l.driver = @driver";
	sqlite3_stmt* stmt2;
	if(sqlite3_prepare_v2(convertedDb,convertedSelect.c_str(),convertedSelect.size(),&stmt2,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),convertedSelect);
	}

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	
	while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
		PacketCarTelemetryData* telemetryData = (PacketCarTelemetryData*) sqlite3_column_blob(stmt,0);
		for(int i=0;i<22;i++){
			int* iFrameIdentifier = (int*)telemetryData->m_header.m_frameIdentifier;
			
			sqlite3_bind_int(stmt2,1,*iFrameIdentifier);
			sqlite3_bind_int(stmt2,2,i+1);
			
			int ret_code2 = sqlite3_step(stmt2);
			
			if(ret_code2 == SQLITE_ROW){
				float steer = telemetryData->m_carTelemetryData[i].m_steer;
				float throttle = telemetryData->m_carTelemetryData[i].m_throttle;
				float breakValue = telemetryData->m_carTelemetryData[i].m_brake;
				int id = sqlite3_column_int(stmt2,0);

				if(bFirst){
					bFirst = false;
				}
				else {
					insertSql += ",";
				}
				insertSql += "(";
				insertSql += std::to_string(id);
				insertSql += ",4,";
				insertSql += std::to_string(steer);
				insertSql += "),(";
				insertSql += std::to_string(id);
				insertSql += ",2,";
				insertSql += std::to_string(throttle);
				insertSql += "),(";
				insertSql += std::to_string(id);
				insertSql += ",3,";
				insertSql += std::to_string(breakValue);
				insertSql += ")";
				numData++;
				if(numData > 1000){
					std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					std::cout << "Float Data Table: Select Statment" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" << std::endl;

					iNumberInserts++;
					std::cout << "Float Data Table: " << rows << "/" << iNumberInserts << std::endl;
					sqlite3_stmt* stmt3;
					begin = std::chrono::steady_clock::now();
					if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt3,NULL) != SQLITE_OK){
						throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
					}
					int ret_code3 = sqlite3_step(stmt3);
					end = std::chrono::steady_clock::now();
					std::cout << "Float Data Table: Insert Statment" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" << std::endl;
					sqlite3_finalize(stmt3);
					if(!(ret_code3 == SQLITE_DONE || ret_code3 == SQLITE_OK)){
						throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
					}
					numData = 0;
					insertSql = "INSERT INTO float_data(reference_unit_id, property_id, float_val) VALUES ";
					bFirst = true;
					begin = std::chrono::steady_clock::now();
				}
			}
			sqlite3_clear_bindings(stmt2);
			sqlite3_reset(stmt2);
		}
	}
	sqlite3_finalize(stmt);

	if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
}
//--------------------------------------
void F1_2020_Converter::insertIntDataTable(sqlite3*f1Db,sqlite3* convertedDb){
	std::cout << "=================== Insert Int Data Table ======================" << std::endl;
	sqlite3_stmt* stmt;
	int ret_code;

	std::string insertSql = "INSERT INTO int_data(reference_unit_id, property_id, int_val) VALUES ";
	std::string selectPacketSql = "SELECT packet FROM packets WHERE packetId = 6";
	if(sqlite3_prepare_v2(f1Db,selectPacketSql.c_str(),selectPacketSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),selectPacketSql);
	}
	int rows = sqlite3_data_count(stmt) ;
	std::cout << "Int Data Table: Total Rows: " << rows << std::endl;
	int iNumberInserts = 0;
	bool bFirst = true;
	int numData = 0;

	std::string convertedSelect = "SELECT ref.id FROM reference_unit ref INNER JOIN lap l ON l.id = ref.lap_id WHERE ref.frame_identifier = @frameIdentifier AND l.driver = @driver";
	sqlite3_stmt* stmt2;
	if(sqlite3_prepare_v2(convertedDb,convertedSelect.c_str(),convertedSelect.size(),&stmt2,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),convertedSelect);
	}

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
		PacketCarTelemetryData* telemetryData = (PacketCarTelemetryData*) sqlite3_column_blob(stmt,0);
		for(int i=0;i<22;i++){
			int* iFrameIdentifier = (int*)telemetryData->m_header.m_frameIdentifier;
			
			sqlite3_bind_int(stmt2,1,*iFrameIdentifier);
			sqlite3_bind_int(stmt2,2,i+1);
			
			int ret_code2 = sqlite3_step(stmt2);
			
			if(ret_code2 == SQLITE_ROW){
				int speed = telemetryData->m_carTelemetryData[i].m_speed;
				int rpm = telemetryData->m_carTelemetryData[i].m_engineRPM;
				int gear = telemetryData->m_carTelemetryData[i].m_gear;
				int id = sqlite3_column_int(stmt2,0);

				if(bFirst){
					bFirst = false;
				}
				else {
					insertSql += ",";
				}
				insertSql += "(";
				insertSql += std::to_string(id);
				insertSql += ",1,";
				insertSql += std::to_string(speed);
				insertSql += "),(";
				insertSql += std::to_string(id);
				insertSql += ",5,";
				insertSql += std::to_string(rpm);
				insertSql += "),(";
				insertSql += std::to_string(id);
				insertSql += ",6,";
				insertSql += std::to_string(gear);
				insertSql += ")";
				numData++;
				if(numData > 1000){
					std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					std::cout << "Int Data Table: Select Statment" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" << std::endl;

					iNumberInserts++;
					std::cout << "Int Data Table: " << rows << "/" << iNumberInserts << std::endl;
					sqlite3_stmt* stmt3;
					begin = std::chrono::steady_clock::now();
					if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt3,NULL) != SQLITE_OK){
						throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
					}
					int ret_code3 = sqlite3_step(stmt3);
					end = std::chrono::steady_clock::now();
					std::cout << "Int Data Table: Insert Statment" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" << std::endl;
					sqlite3_finalize(stmt3);
					if(!(ret_code3 == SQLITE_DONE || ret_code3 == SQLITE_OK)){
						throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
					}
					numData = 0;
					insertSql = "INSERT INTO int_data(reference_unit_id, property_id, int_val) VALUES ";
					bFirst = true;
					begin = std::chrono::steady_clock::now();
				}
			}
			sqlite3_clear_bindings(stmt2);
			sqlite3_reset(stmt2);
		}
	}
	sqlite3_finalize(stmt);

	if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
}
//--------------------------------------
void F1_2020_Converter::insertVec3DataTable(sqlite3*f1Db,sqlite3* convertedDb){
	std::cout << "=================== Insert Vec Data Table ======================" << std::endl;
	sqlite3_stmt* stmt;
	int ret_code;

	std::string insertSql = "INSERT INTO vec_data(reference_unit_id, property_id, x_val,y_val,z_val) VALUES ";
	std::string selectPacketSql = "SELECT packet FROM packets WHERE packetId = 0";
	if(sqlite3_prepare_v2(f1Db,selectPacketSql.c_str(),selectPacketSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),selectPacketSql);
	}
	int rows = sqlite3_data_count(stmt) ;
	std::cout << "Vec Data Table: Total Rows: " << rows << std::endl;
	int iNumberInserts = 0;
	bool bFirst = true;
	int numData = 0;

	std::string convertedSelect = "SELECT ref.id FROM reference_unit ref INNER JOIN lap l ON l.id = ref.lap_id WHERE ref.frame_identifier = @frameIdentifier AND l.driver = @driver";
	sqlite3_stmt* stmt2;
	if(sqlite3_prepare_v2(convertedDb,convertedSelect.c_str(),convertedSelect.size(),&stmt2,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),convertedSelect);
	}

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW){
		PacketMotionData* motionData = (PacketMotionData*) sqlite3_column_blob(stmt,0);

		for(int i=0;i<22;i++){
			int* iFrameIdentifier = (int*)motionData->m_header.m_frameIdentifier;
			
			sqlite3_bind_int(stmt2,1,*iFrameIdentifier);
			sqlite3_bind_int(stmt2,2,i+1);
			
			int ret_code2 = sqlite3_step(stmt2);
			
			if(ret_code2 == SQLITE_ROW){
				float posX = motionData->m_carMotionData[i].m_worldPositionX;
				float posY = motionData->m_carMotionData[i].m_worldPositionY;
				float posZ = motionData->m_carMotionData[i].m_worldPositionZ;
				int id = sqlite3_column_int(stmt2,0);

				if(bFirst){
					bFirst = false;
				}
				else {
					insertSql += ",";
				}
				insertSql += "(";
				insertSql += std::to_string(id);
				insertSql += ",1,";
				insertSql += std::to_string(posX);
				insertSql += ",";
				insertSql += std::to_string(posY);
				insertSql += ",";
				insertSql += std::to_string(posZ);
				insertSql += ")";
				numData++;
				if(numData > 1000){
					std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					std::cout << "Vec Data Table: Select Statment" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" << std::endl;

					iNumberInserts++;
					std::cout << "Vec Data Table: " << rows << "/" << iNumberInserts << std::endl;
					sqlite3_stmt* stmt3;
					begin = std::chrono::steady_clock::now();
					if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt3,NULL) != SQLITE_OK){
						throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
					}
					int ret_code3 = sqlite3_step(stmt3);
					end = std::chrono::steady_clock::now();
					std::cout << "Vec Data Table: Insert Statment" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" << std::endl;
					sqlite3_finalize(stmt3);
					if(!(ret_code3 == SQLITE_DONE || ret_code3 == SQLITE_OK)){
						throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
					}
					numData = 0;
					insertSql = "INSERT INTO vec_data(reference_unit_id, property_id, x_val,y_val,z_val) VALUES ";
					bFirst = true;
					begin = std::chrono::steady_clock::now();
				}
			}
			sqlite3_clear_bindings(stmt2);
			sqlite3_reset(stmt2);
		}
		
	}
	sqlite3_finalize(stmt);

	if(sqlite3_prepare_v2(convertedDb,insertSql.c_str(),insertSql.size(),&stmt,NULL) != SQLITE_OK){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
	ret_code = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(ret_code != SQLITE_DONE){
		throw SQLErrorException(sqlite3_errmsg(convertedDb),insertSql);
	}
}


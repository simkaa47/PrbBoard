/*
 * vozvratKovshaFb.c
 *
 *  Created on: 10 янв. 2023 г.
 *      Author: Serafim
 */
#include <vozvratkovshaFB.h>

void Vozvrat_Kovsha(Vozvrat_Kovsha_Struct *var)
{
	// Возврат ковша
	if (var->startflag) {
		var->rev = 0;
		var->forw = 0;
		var->busy = 1;
		var->error = 0;
		var->status = 1;
		var->startflag = 0;
	}

	var->speedout = var->setSpeed;
	var->statusout = var->status;

	switch (var->type) {
	// пересечной
		case 1:
		case 2:
		case 6:
			switch (var->status) {
				case 1:
					if (!var->homePos) {
						if (var->sqStop1) {
							var->rev = 1;
							var->moveDirection = 0;
							var->forw = 0;
							var->status = 11;
						} else {
							var->forw = 1;
							var->moveDirection = 1;
							var->rev = 0;
							var->status = 12;
						}
					} else {
						var->status = 2;
					}
					break;
				case 11:
					if (!var->sqStop1) {
						var->status = 2;
					} else if(var->sqStop2 || var->workPos) {
						var->rev = 0;
						var->forw  =0;
						var->busy = 0;
						var->error = 6;
						var->status = 0;
					}
					break;
				case 12:
					if (!var->sqStop2) {
						var->status = 2;
					} else if(var->sqStop1 || var->homePos) {
						var->rev = 0;
						var->forw  =0;
						var->busy = 0;
						var->error = 6;
						var->status = 0;
					}
					break;
				case 2:
					if (var->homePos || var->workPos) {
						var->rev = 0;
						var->forw  =0;
						var->busy = 0;
						var->status = 0;
					}
					if (var->sqStop1 || var->sqStop2) {
						var->rev = 0;
						var->forw  =0;
						var->busy = 0;
						var->error = 6;
						var->status = 0;
					}
					break;
				default:
					break;
			}
			break;
		// маятниковый или роторный делитель
		case 3:
		case 4:
			switch (var->status) {
				case 1:
					if (!var->homePos) {
						if (var->forw_rev) {
							var->forw = 0;
							var->rev = 1;
							var->status = 2;
						} else {
							var->forw = 1;
							var->rev = 0;
							var->status = 2;
						}
					} else {
						var->status = 2;
					}
					break;
				case 2:
					if (var->homePos) {
						var->rev = 0;
						var->forw = 0;
						var->busy = 0;
						var->status = 0;
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}


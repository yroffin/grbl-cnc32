import { Injectable } from '@angular/core';
import { Effect, Actions, ofType } from '@ngrx/effects';
import { Observable } from 'rxjs';
import { of } from 'rxjs';
import { switchMap } from 'rxjs/operators';
import { Store } from '@ngrx/store';
import { GrblService } from '../services/grbl.service';
import { Status } from '../models/grbl';

@Injectable({
    providedIn: 'root'
})
export class StoreService {

    constructor(private store: Store<IAppState>) {
    }

    // all reducers
    static reducers = {
        status: (state: Status = {}, action: StatusAction): Status => {
            switch (action.type) {
                case AllActionType.setStatusOk:
                    return action.payload;
                default:
                    console.log(`action ${action.type}`);
                    return state;
            }
        }
    };

    // all dispatch
    dispatchSetStatus() {
        this.store.dispatch({
            type: AllActionType.setStatus,
            payload: status
        });
    }

    getStatus() {
        return this.store.select(s => s.status);
    }
}

// internal state structure
export interface IAppState {
    status: Status;
}

// anum of all action
export enum AllActionType {
    setStatus = 'setStatus',
    setStatusOk = 'setStatusOk'
}

// all action
export class StatusAction {
    type: AllActionType;
    constructor(public payload: Status) { }
}

@Injectable()
export class ActionListenerEffects {

    constructor(private grblService: GrblService, private actions$: Actions) { }

    @Effect()
    addAction$: Observable<StatusAction> = this.actions$.pipe(
        ofType<StatusAction>(AllActionType.setStatus),
        switchMap((action: StatusAction) => {
            /**
             * action logique here
             */
            // return this.grblService.getStatus();
            return of<StatusAction>({
                type: AllActionType.setStatusOk, payload: {
                    working: {
                        mpos: {
                            x: Math.floor(Math.random() * Math.floor(100)),
                            y: Math.floor(Math.random() * Math.floor(100)),
                            z: Math.floor(Math.random() * Math.floor(100))
                        },
                        wpos: {
                            x: Math.floor(Math.random() * Math.floor(100)),
                            y: Math.floor(Math.random() * Math.floor(100)),
                            z: Math.floor(Math.random() * Math.floor(100))
                        },
                        modal: {
                            abs: true,
                            metric: true
                        }
                    }
                }
            });
        })
    );
}

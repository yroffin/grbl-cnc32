import { Injectable } from '@angular/core';
import { Effect, Actions, ofType } from '@ngrx/effects';
import { Observable } from 'rxjs';
import { of } from 'rxjs';
import { map, switchMap } from 'rxjs/operators';
import { Store } from '@ngrx/store';
import { GrblService } from '../services/grbl.service';
import { Status } from '../models/grbl';
import { _ACTIVE_RUNTIME_CHECKS } from '@ngrx/store/src/tokens';
import * as _ from 'lodash';

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
                case AllActionType.getStatusOk:
                    return action.payload;
                default:
                    return state;
            }
        },
        commands: (state: string[] = [], action: StatusAction): string[] => {
            switch (action.type) {
                case AllActionType.addCommandOk:
                    return _.union(state, [action.payload]);
                default:
                    return state;
            }
        }
    };

    // all dispatch
    dispatchGetStatus() {
        this.store.dispatch({
            type: AllActionType.getStatus
        });
    }

    dispatchSetStatus(body: string) {
        this.store.dispatch({
            type: AllActionType.addCommand,
            payload: body
        });
    }

    getStatus() {
        return this.store.select(s => s.status);
    }

    getCommands() {
        return this.store.select(s => s.commands);
    }
}

// internal state structure
export interface IAppState {
    status: Status;
    commands: string[];
}

// anum of all action
export enum AllActionType {
    getStatus = 'getStatus',
    getStatusOk = 'getStatusOk',
    addCommand = 'addCommand',
    addCommandOk = 'addCommandOk'
}

// all action
export class StatusAction {
    type: AllActionType;
    constructor(public payload: any) { }
}

@Injectable()
export class ActionListenerEffects {

    constructor(private grblService: GrblService, private actions$: Actions) { }

    @Effect()
    getStatus$: Observable<StatusAction> = this.actions$.pipe(
        ofType<StatusAction>(AllActionType.getStatus),
        switchMap((action) => {
            return this.grblService.getStatus();
        }),
        switchMap((payload: Status) => {
            return of({
                type: AllActionType.getStatusOk,
                payload
            });
        })
    );

    @Effect()
    addCommand$: Observable<StatusAction> = this.actions$.pipe(
        ofType<StatusAction>(AllActionType.addCommand),
        switchMap((action) => {
            return this.grblService.addCommand(action.payload);
        }),
        switchMap((payload: string) => {
            return of({
                type: AllActionType.addCommandOk,
                payload
            });
        })
    );
}

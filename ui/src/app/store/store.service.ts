import { Injectable } from '@angular/core';
import { Effect, Actions, ofType } from '@ngrx/effects';
import { Observable } from 'rxjs';
import { of } from 'rxjs';
import { map, switchMap, catchError } from 'rxjs/operators';
import { Store } from '@ngrx/store';
import { GrblService } from '../services/grbl.service';
import { Status, StlInfo, Vector } from '../models/grbl';
import { _ACTIVE_RUNTIME_CHECKS } from '@ngrx/store/src/tokens';
import * as _ from 'lodash';
import { Message } from 'primeng/api/message';

@Injectable({
    providedIn: 'root'
})
export class StoreService {

    constructor(private store: Store<IAppState>) {
    }

    // all reducers
    static reducers = {
        status: (state: Status = {}, action: AllAction): Status => {
            switch (action.type) {
                case AllActionType.getStatusOk:
                    return action.payload;
                default:
                    return state;
            }
        },
        commands: (state: string[] = [], action: AllAction): string[] => {
            switch (action.type) {
                case AllActionType.addCommandOk:
                    return _.union(state, [action.payload]);
                default:
                    return state;
            }
        },
        stlInfo: (state: StlInfo = {}, action: AllAction): StlInfo => {
            switch (action.type) {
                case AllActionType.stlInfo:
                    return action.payload;
                default:
                    return state;
            }
        },
        errors: (state: Message = {}, action: AllAction): Message => {
            switch (action.type) {
                case AllActionType.getStatusKo:
                    return action.payload;
                case AllActionType.addCommandKo:
                    return action.payload;
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

    dispatchAddCommand(body: string) {
        this.store.dispatch({
            type: AllActionType.addCommand,
            payload: body
        });
    }

    dispatchStlInfo(min: Vector, max: Vector) {
        this.store.dispatch({
            type: AllActionType.stlInfo,
            payload: {
                min,
                max
            }
        });
    }

    getStatus() {
        return this.store.select(s => s.status);
    }

    getCommands() {
        return this.store.select(s => s.commands);
    }

    getStlInfo() {
        return this.store.select(s => s.stlInfo);
    }

    getErrors() {
        return this.store.select(s => s.errors);
    }
}

// internal state structure
export interface IAppState {
    status: Status;
    commands: string[];
    stlInfo: StlInfo;
    errors: any[];
}

// anum of all action
export enum AllActionType {
    getStatus = 'getStatus',
    getStatusOk = 'getStatusOk',
    getStatusKo = 'getStatusKo',
    addCommand = 'addCommand',
    addCommandOk = 'addCommandOk',
    addCommandKo = 'addCommandKo',
    stlInfo = 'stlInfo',
    stlInfoOk = 'stlInfoOk'
}

// all action
export class AllAction {
    type: AllActionType;
    constructor(public payload: any) { }
}

@Injectable()
export class ActionListenerEffects {

    constructor(private grblService: GrblService, private actions$: Actions) { }

    @Effect()
    getStatus$: Observable<AllAction> = this.actions$.pipe(
        ofType<AllAction>(AllActionType.getStatus),
        switchMap((action) =>
            this.grblService.getStatus().pipe(
                switchMap((payload: Status) => {
                    return of({
                        type: AllActionType.getStatusOk,
                        payload
                    });
                }),
                catchError((error) => {
                    return of({
                        type: AllActionType.getStatusKo,
                        payload: { severity: 'error', summary: 'Error Message', detail: JSON.stringify(error) }
                    });
                })
            )
        )
    );

    @Effect()
    addCommand$: Observable<AllAction> = this.actions$.pipe(
        ofType<AllAction>(AllActionType.addCommand),
        switchMap((action) =>
            this.grblService.addCommand(action.payload).pipe(
                switchMap((payload: string) => {
                    return of({
                        type: AllActionType.addCommandOk,
                        payload
                    });
                }),
                catchError((error) => {
                    return of({
                        type: AllActionType.addCommandKo,
                        payload: { severity: 'error', summary: 'Error Message', detail: JSON.stringify(error) }
                    });
                })
            )
        )
    );
}

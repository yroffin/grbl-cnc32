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
import { MessageService } from 'primeng/api';

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
        files: (state: string[] = [], action: AllAction): string[] => {
            switch (action.type) {
                case AllActionType.browseFileOk:
                    return _.clone(action.payload);
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
        messages: (state: Message = {}, action: AllAction): Message => {
            switch (action.type) {
                case AllActionType.getStatusKo:
                    return action.payload;
                case AllActionType.addCommandKo:
                    return action.payload;
                case AllActionType.printFileKo:
                    return action.payload;
                case AllActionType.printFileOk:
                    return {
                        severity: 'success', summary: 'Printing ok', detail: JSON.stringify(action.payload)
                    };
                case AllActionType.deleteFileKo:
                    return action.payload;
                case AllActionType.deleteFileOk:
                    return {
                        severity: 'success', summary: 'Delete ok', detail: JSON.stringify(action.payload)
                    };
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

    dispatchPrintFile(body: string) {
        this.store.dispatch({
            type: AllActionType.printFile,
            payload: body
        });
    }

    dispatchDeleteFile(body: string) {
        this.store.dispatch({
            type: AllActionType.deleteFile,
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

    dispatchBrowseFiles() {
        this.store.dispatch({
            type: AllActionType.browseFile
        });
    }

    getStatus() {
        return this.store.select(s => s.status);
    }

    getFiles() {
        return this.store.select(s => s.files);
    }

    getCommands() {
        return this.store.select(s => s.commands);
    }

    getStlInfo() {
        return this.store.select(s => s.stlInfo);
    }

    getMessages() {
        return this.store.select(s => s.messages);
    }
}

// internal state structure
export interface IAppState {
    status: Status;
    commands: string[];
    files: string[];
    stlInfo: StlInfo;
    messages: any[];
    infos: any[];
}

// anum of all action
export enum AllActionType {
    getStatus = 'getStatus',
    getStatusOk = 'getStatusOk',
    getStatusKo = 'getStatusKo',
    browseFile = 'browseFile',
    browseFileOk = 'browseFileOk',
    browseFileKo = 'browseFileKo',
    addCommand = 'addCommand',
    addCommandOk = 'addCommandOk',
    addCommandKo = 'addCommandKo',
    printFile = 'printFile',
    printFileOk = 'printFileOk',
    printFileKo = 'printFileKo',
    deleteFile = 'deleteFile',
    deleteFileOk = 'deleteFileOk',
    deleteFileKo = 'deleteFileKo',
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

    @Effect()
    browseFile$: Observable<AllAction> = this.actions$.pipe(
        ofType<AllAction>(AllActionType.browseFile),
        switchMap((action) =>
            this.grblService.browseFile().pipe(
                switchMap((payload: string[]) => {
                    return of({
                        type: AllActionType.browseFileOk,
                        payload
                    });
                }),
                catchError((error) => {
                    return of({
                        type: AllActionType.browseFileKo,
                        payload: { severity: 'error', summary: 'Error Message', detail: JSON.stringify(error) }
                    });
                })
            )
        )
    );

    @Effect()
    printFile$: Observable<AllAction> = this.actions$.pipe(
        ofType<AllAction>(AllActionType.printFile),
        switchMap((action) =>
            this.grblService.printFile(action.payload.name).pipe(
                switchMap((payload: any) => {
                    return of({
                        type: AllActionType.printFileOk,
                        payload
                    });
                }),
                catchError((error) => {
                    return of({
                        type: AllActionType.printFileKo,
                        payload: { severity: 'error', summary: 'Error Message', detail: JSON.stringify(error) }
                    });
                })
            )
        )
    );

    @Effect()
    deleteFile$: Observable<AllAction> = this.actions$.pipe(
        ofType<AllAction>(AllActionType.deleteFile),
        switchMap((action) =>
            this.grblService.deleteFile(action.payload.name).pipe(
                switchMap((payload: any) => {
                    return of({
                        type: AllActionType.deleteFileOk,
                        payload
                    });
                }),
                catchError((error) => {
                    return of({
                        type: AllActionType.deleteFileKo,
                        payload: { severity: 'error', summary: 'Error Message', detail: JSON.stringify(error) }
                    });
                })
            )
        )
    );
}

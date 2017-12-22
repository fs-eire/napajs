// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

//import * as path from 'path';
import * as zone from './zone';
//import * as transport from '../transport';
//import * as v8 from '../v8';

interface FunctionSpec {
    module: string;
    function: string;
    arguments: any[];
    options: zone.CallOptions;
    //transportContext: transport.TransportContext;
}

declare var __in_napa: boolean;

/// <summary> Helper function to workaround possible delay in Promise resolve/reject when working with Node event loop.
/// See https://github.com/audreyt/node-webworker-threads/issues/123#issuecomment-254019552
/// </summary>
function runImmediately(func : () => void) {
    if (typeof __in_napa === 'undefined') {
        // In node.
        setImmediate(func);
    } else  {
        // In napa workers.
        func();
    }
}

/// <summary> Zone consists of Napa isolates. </summary>
export class ZoneImpl implements zone.Zone {
    private _nativeZone: any;

    constructor(nativeZone: any) {
        this._nativeZone = nativeZone;
    }

    public get id(): string {
        return this._nativeZone.getId();
    }

    public toJSON(): any {
        return { id: this.id, type: this.id === 'node'? 'node': 'napa' };
    }

    public broadcast(arg1: any, arg2?: any) : Promise<void> {
        throw new Error('Should not run to this line');
    }

    public execute(arg1: any, arg2?: any, arg3?: any, arg4?: any) : Promise<zone.Result> {
        throw new Error('Should not run to this line');
    }

}
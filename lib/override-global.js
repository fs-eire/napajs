// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

let initialized = false;

function override_global() {
    // Prepare global store __napa_internal
    const store_internal = (typeof __in_napa === 'undefined')
                            ? napa.store.create('__napa_internal')
                            : napa.store.get('__napa_internal');

    // Wrap process.dlopen() with lock
    process.dlopen = (function () {
        if (typeof __in_napa === 'undefined') {
            store_internal.set('lock_dlopen', napa.sync.createLock());
        }

        const old_dlopen = process.dlopen;
        const lock = store_internal.get('lock_dlopen');
        const dlopen = function (arg1, arg2, arg3) {
            return lock.guardSync(() => {
                return old_dlopen(arg1, arg2, arg3);
            });
        };
        return dlopen;
    })();

}

if (typeof napa === 'object' && !initialized) {
    override_global();
    initialized = true;
}

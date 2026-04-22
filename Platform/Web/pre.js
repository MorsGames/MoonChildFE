Module.preRun = Module.preRun || [];
Module.preRun.push(function() {
    const mountPoint = '/persistent';
    try {
        FS.mkdir(mountPoint);
    }
    catch (e) {}

    try {
        FS.mount(IDBFS, {}, mountPoint);
    }
    catch (e) {}

    const dependency = 'moonchildfe-userdata';
    Module.addRunDependency(dependency);
    FS.syncfs(true, function(err) {
        if (err)
            console.warn('Failed to load persistent storage!', err);
        Module.removeRunDependency(dependency);
    });
});

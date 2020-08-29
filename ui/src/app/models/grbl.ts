export class Status {
    stored?: Dump;
    working?: Dump;
}

export class Command {
    tryWrite?: string;
    forceWrite?: string;
    reset?: string;
}

export class StlInfo {
    min?: Vector;
    max?: Vector;
}

export class ConfigJsonSpeed {
    l1: number;
    l2: number;
    l3: number;
}

export class ConfigJsonWorkbench {
    width: number;
    height: number;
    length: number;
}

export class ConfigJsonSys {
    lang: string;
    speed: ConfigJsonSpeed;
    workbench: ConfigJsonWorkbench;
}

export class ConfigJson {
    sys?: ConfigJsonSys;
}

export class Dump {
    mpos: Vector;
    wpos: Vector;
    modal: Modal;
}

export class Vector {
    x: number;
    y: number;
    z: number;
}

export class Modal {
    metric: boolean;
    abs: boolean;
}

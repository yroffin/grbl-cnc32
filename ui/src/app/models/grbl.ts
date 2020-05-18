export class Status {
    stored?: Dump;
    working?: Dump;
}

export class StlInfo {
    min?: Vector;
    max?: Vector;
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

import { Component, OnInit, ViewChild, ElementRef, EventEmitter, Output, AfterViewInit, ChangeDetectorRef, HostListener } from '@angular/core';
import * as paper from 'paper';
import { FormGroup, FormBuilder } from '@angular/forms';
import { MessageService } from 'primeng/api';
import * as _ from 'lodash';

export enum PaperAction {
  StoredWithShift
}

@Component({
  selector: 'app-paperjs',
  templateUrl: './paperjs.component.html',
  styleUrls: ['./paperjs.component.css'],
  providers: [
    MessageService
  ]
})
export class PaperjsComponent implements OnInit, AfterViewInit {

  @ViewChild('paperView') paperCanvas: ElementRef;

  store: any;
  container: HTMLElement;
  registerForm: FormGroup;
  x = 0;
  y = 0;
  z = 0;

  oldZoom = 1;
  zoom = 1;

  private scope: paper.PaperScope;
  private project: paper.Project;
  private formBuilder = new FormBuilder();

  shiftIsDown = false;

  @Output() flag: EventEmitter<any> = new EventEmitter();
  @Output() ready: EventEmitter<any> = new EventEmitter();

  constructor(private cdr: ChangeDetectorRef, private messageService: MessageService) {
    this.registerForm = this.formBuilder.group({
      x: [],
      y: [],
      z: [],
    });
  }

  @HostListener('window:keydown', ['$event'])
  handleKeyDown(event: KeyboardEvent) {
    if (event.key === 'Shift') {
      console.log('down');
      this.shiftIsDown = true;
    }
  }

  @HostListener('window:keyup', ['$event'])
  handleKeyUp(event: KeyboardEvent) {
    if (event.key === 'Shift') {
      console.log('down');
      this.shiftIsDown = false;
    }
  }

  ngOnInit() {
    // For using paper libs
    this.scope = new paper.PaperScope();
  }

  ngAfterViewInit(): void {
    const height = this.paperCanvas.nativeElement.offsetHeight;
    const width = this.paperCanvas.nativeElement.offsetWidth;

    this.project = new paper.Project(this.paperCanvas.nativeElement);
    this.project.view.center = new paper.Point(width / 8, height / 8);
    this.project.view.scale(this.zoom, -this.zoom);

    this.project.view.onMouseMove = (event) => {
      this.onMouseMove(event);
    };

    this.drawGrid(width, height, 10, 1);
    this.ready.emit(this.project);
    // Finalize view init in async
    setTimeout(
      () => {
        this.setZoom(3);
        this.messageService.add({ severity: 'success', summary: 'Native', detail: `Canvas size ${width},${height}` });
      }, 1000);
  }

  onZoomChange(event: any) {
    this.setZoom(this.zoom);
  }

  onCapture() {
    if (this.shiftIsDown === false) { return; }
    this.store = {
      x: this.x ? this.x.toFixed(3) : 0.,
      y: this.y ? this.y.toFixed(3) : 0.,
      status: PaperAction.StoredWithShift
    };
    this.messageService.add({ severity: 'success', summary: 'Move', detail: `To ${this.store.x},${this.store.y}` });
    this.flag.emit(this.store);
  }

  private setZoom(zoom: number) {
    this.project.view.scale(1 / this.oldZoom, 1 / -this.oldZoom);
    this.project.view.scale(zoom, -zoom);
    this.oldZoom = zoom;
    this.zoom = zoom;
    const height = this.paperCanvas.nativeElement.offsetHeight;
    const width = this.paperCanvas.nativeElement.offsetWidth;
    this.project.view.center = new paper.Point(width / (this.zoom * 2.5), height / (this.zoom * 2));
  }

  private onMouseMove(event: any) {
    this.x = event.point.x;
    this.y = event.point.y;
    this.cdr.detectChanges();
    return false;
  }

  private drawGrid(width: number, height: number, heavy: number, light: number) {
    const canvas = new paper.Path.Rectangle(
      new paper.Point(0, 0),
      new paper.Point(width, height)
    );

    const ordonnee = new paper.Path.Line({
      from: new paper.Point(0, -height),
      to: new paper.Point(0, height),
      strokeColor: 'red',
      strokeWidth: 3
    });

    const abscisse = new paper.Path.Line({
      from: new paper.Point(-width, 0),
      to: new paper.Point(width, 0),
      strokeColor: 'blue',
      strokeWidth: 3
    });

    for (let x = -width; x < width; x += light) {
      const line = new paper.Path.Line({
        from: new paper.Point(x, 0),
        to: new paper.Point(x, height),
        strokeColor: 'lightblue',
        strokeWidth: (x % heavy === 0) ? 0.2 : 0.1
      });
    }

    for (let y = -height; y < height; y += light) {
      const line = new paper.Path.Line({
        from: new paper.Point(0, y),
        to: new paper.Point(height, y),
        strokeColor: 'lightblue',
        strokeWidth: (y % heavy === 0) ? 0.2 : 0.1
      });
    }
  }
}

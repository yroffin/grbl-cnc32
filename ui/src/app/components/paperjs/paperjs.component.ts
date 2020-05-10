import { Component, OnInit, ViewChild, ElementRef, EventEmitter, Output, AfterViewInit, ChangeDetectorRef } from '@angular/core';
import * as paper from 'paper';
import { FormGroup, FormBuilder } from '@angular/forms';

@Component({
  selector: 'app-paperjs',
  templateUrl: './paperjs.component.html',
  styleUrls: ['./paperjs.component.css']
})
export class PaperjsComponent implements OnInit, AfterViewInit {

  @ViewChild('paperView') paperCanvas: ElementRef;
  @ViewChild('paperCube') paperCube: ElementRef;

  store: any;
  container: HTMLElement;
  registerForm: FormGroup;
  x = 0;
  y = 0;
  z = 0;

  private scope: paper.PaperScope;
  private project: paper.Project;
  private formBuilder = new FormBuilder();

  @Output() zoomin: EventEmitter<any> = new EventEmitter();
  @Output() zoomout: EventEmitter<any> = new EventEmitter();
  @Output() flag: EventEmitter<any> = new EventEmitter();

  constructor(private cdr: ChangeDetectorRef) {
    this.registerForm = this.formBuilder.group({
      x: [],
      y: [],
      z: [],
    });
  }

  ngOnInit() {
    // For using paper libs
    this.scope = new paper.PaperScope();
  }

  ngAfterViewInit(): void {
    const height = this.paperCanvas.nativeElement.offsetHeight;
    const width = this.paperCanvas.nativeElement.offsetWidth;
    console.log(width, height);

    this.project = new paper.Project(this.paperCanvas.nativeElement);
    this.project.view.center = new paper.Point(200, 100);
    this.project.view.scale(3, -3);

    this.project.view.onMouseMove = (event) => {
      this.onMouseMove(event);
    };

    this.project.view.onMouseUp = (event) => {
      this.onMouseUp(event);
    };

    this.project.view.onDoubleClick = (event) => {
      this.onDoubleClick(event);
    };

    this.drawGrid(width, height, 10, 1);

    const rect = new paper.Path.Rectangle({
      from: new paper.Point(0, 0),
      to: new paper.Size(245, 80),
      strokeColor: 'black',
      strokeWidth: 1
    });
  }

  onWheel(event: any) {
    event.preventDefault();
    if (event.deltaY < 0) {
      this.zoomout.emit(event);
    } else if (event.deltaY > 0) {
      this.zoomin.emit(event);
    }
  }

  onCapture() {
    this.store = {
      x: this.x ? this.x.toFixed(3) : 0.,
      y: this.y ? this.y.toFixed(3) : 0.,
    };
  }

  move() {
    this.flag.emit({
      x: this.store.x,
      y: this.store.y,
      event: 'move'
    });
  }

  private onMouseMove(event: any) {
    this.x = event.point.x;
    this.y = event.point.y;
    this.cdr.detectChanges();
    return false;
  }

  private onMouseUp(event: any) {
  }

  private onDoubleClick(event: any) {
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

import { Component, OnInit, OnDestroy } from '@angular/core';
import { GrblService } from '../../services/grbl.service';
import { Status, StlInfo } from '../../models/grbl';
import * as paper from 'paper';
import { MessageService, Message } from 'primeng/api';
import { StoreService } from '../../store/store.service';
import { TerminalService } from 'primeng/terminal';
import { Subscription } from 'rxjs';
import { _ACTIVE_RUNTIME_CHECKS } from '@ngrx/store/src/tokens';
import * as _ from 'lodash';

@Component({
  selector: 'app-console',
  templateUrl: './console.component.html',
  styleUrls: ['./console.component.css'],
  providers: [
    MessageService,
    TerminalService
  ]
})
export class ConsoleComponent implements OnInit, OnDestroy {

  subscriptions: Subscription[] = [];

  status: Status;
  commands: string[];
  files: string[];
  cols = [{
    'header': 'Nom',
    'field': 'name'
  }];
  stlInfo: StlInfo;

  step = 1;
  toolsXY: paper.Path.Circle;
  toolsZ: paper.Path.Rectangle;

  planXY: paper.Path.Rectangle;
  planZ: paper.Path.Rectangle;

  xy: paper.Project;
  z: paper.Project;

  constructor(
    private terminalService: TerminalService,
    private storeService: StoreService,
    private messageService: MessageService) {
    // Error handling
    this.subscriptions.push(storeService.getMessages().subscribe(
      (error: any) => {
        this.messageService.add(error);
      }
    ));
    // Handle stl info
    this.subscriptions.push(storeService.getStlInfo().subscribe(
      (info: StlInfo) => {
        if (info.min) {
          const maxX = Math.abs(info.max.x - info.min.x);
          const maxY = Math.abs(info.max.y - info.min.y);
          const maxZ = Math.abs(info.max.z - info.min.z);
          this.stlInfo = {
            min: {
              x: 0,
              y: 0,
              z: 0
            },
            max: {
              x: maxX,
              y: maxZ,
              z: maxY
            }
          };
        }
      }
    ));
    // Handle status
    this.subscriptions.push(storeService.getStatus().subscribe(
      (status) => {
        if (status.working) {
          this.status = status;
          if (this.toolsXY) {
            const deltaXY = new paper.Point(this.status.working.wpos.x, this.status.working.wpos.y).subtract(this.toolsXY.position);
            this.toolsXY.translate(deltaXY);
          }
          if (this.toolsZ) {
            const deltaZ = new paper.Point(
              this.status.working.wpos.x,
              this.status.working.wpos.z).subtract(this.toolsZ.bounds.topCenter);
            this.toolsZ.translate(deltaZ);
          }
        }
      }
    ));
    // Handle files
    this.subscriptions.push(storeService.getFiles().subscribe(
      (files) => {
        console.log('file', files);
        this.files = _.map(_.filter(files, (file: string) => {
          // filter on gcode files
          return file.endsWith('gcode')
        }), (file: string) => {
          return {
            name: file
          }
        });
        console.log(this.files);
      }
    ));
    // Emit command from terminal
    this.subscriptions.push(this.terminalService.commandHandler.subscribe(command => {
      this.command(command);
    }));
  }

  ngOnInit(): void {
    this.storeService.dispatchBrowseFiles();
  }

  ngOnDestroy() {
    _.each(this.subscriptions, (subscription) => {
      subscription.unsubscribe();
    });
  }

  refresh(event: any) {
    this.storeService.dispatchGetStatus();
  }

  refreshXY(event: any) {
    this.planXY.segments = [
      new paper.Segment(new paper.Point(0, 0)),
      new paper.Segment(new paper.Point(this.stlInfo.max.x, 0)),
      new paper.Segment(new paper.Point(this.stlInfo.max.x, this.stlInfo.max.y)),
      new paper.Segment(new paper.Point(0, this.stlInfo.max.y))];
    this.xy.view.update();
  }

  refreshZ(event: any) {
    this.planZ.segments = [
      new paper.Segment(new paper.Point(0, 0)),
      new paper.Segment(new paper.Point(this.stlInfo.max.y, 0)),
      new paper.Segment(new paper.Point(this.stlInfo.max.y, this.stlInfo.max.z)),
      new paper.Segment(new paper.Point(0, this.stlInfo.max.z))];
    this.z.view.update();
  }

  selectXY(event: any) {
    this.command(`X${event.x} Y${event.y}`);
  }

  drawXY(project: paper.Project) {
    project.activate();
    this.xy = project;
    this.planXY = new paper.Path.Rectangle({
      from: new paper.Point(0, 0),
      to: new paper.Size(0, 0),
      strokeColor: 'black',
      strokeWidth: 1
    });
    this.toolsXY = new paper.Path.Circle({
      center: new paper.Point(0, 0),
      radius: 10,
      strokeColor: 'black',
      strokeWidth: 1
    });
  }

  selectZ(event: any) {
    this.command(`Z${event.y}`);
  }

  drawZ(project: paper.Project) {
    project.activate();
    this.z = project;
    this.planZ = new paper.Path.Rectangle({
      from: new paper.Point(0, 0),
      to: new paper.Size(0, 0),
      strokeColor: 'black',
      strokeWidth: 1
    });
    this.toolsZ = new paper.Path.Rectangle({
      from: new paper.Point(-5, 0),
      to: new paper.Size(5, 20),
      strokeColor: 'black',
      strokeWidth: 1
    });
  }

  handleSelect(file: any) {
    this.storeService.dispatchPrintFile(file);
  }
  setHome(event: any) {
    this.command('$H');
  }
  setUnlock(event: any) {
    this.command('$X');
  }
  setReset(event: any) {
    this.command(String.fromCharCode(0x18, 0x10));
  }
  setPause(event: any) {
    this.command('!');
  }
  setResume(event: any) {
    this.command('~');
  }
  setStatus(event: any) {
    this.command('?');
  }

  setX(event: any) {
    this.command('G10 L20 P1 X0');
  }
  setY(event: any) {
    this.command('G10 L20 P1 Y0');
  }
  setZ(event: any) {
    this.command('G10 L20 P1 Z0');
  }
  setAll(event: any) {
    this.command('G10 L20 P1 X0 Y0 Z0');
  }
  setDefinedHome(event: any) {
    this.command('G28.1');
  }
  setRight(event: any) {
    this.command(`$J=G91 G21 X${this.step.toFixed(3)} F100`);
  }
  setLeft(event: any) {
    this.command(`$J=G91 G21 X-${this.step.toFixed(3)} F100`);
  }
  setBack(event: any) {
    this.command(`$J=G91 G21 Y${this.step.toFixed(3)} F100`);
  }
  setFront(event: any) {
    this.command(`$J=G91 G21 Y-${this.step.toFixed(3)} F100`);
  }
  setStep(event: any) {
  }
  setUp(event: any) {
    this.command(`$J=G91 G21 Z${this.step.toFixed(3)} F100`);
  }
  setDown(event: any) {
    this.command(`$J=G91 G21 Z-${this.step.toFixed(3)} F100`);
  }
  setOk(event: any) {
    this.command('ok');
  }

  private command(command: string) {
    this.storeService.dispatchAddCommand(command);
  }
}

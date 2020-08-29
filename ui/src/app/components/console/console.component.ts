import { Component, OnInit, OnDestroy, ViewChild, AfterViewInit } from '@angular/core';
import { Status, StlInfo, ConfigJson } from '../../models/grbl';
import * as paper from 'paper';
import { MessageService, Message } from 'primeng/api';
import { StoreService } from '../../store/store.service';
import { TerminalService } from 'primeng/terminal';
import { Subscription } from 'rxjs';
import { _ACTIVE_RUNTIME_CHECKS } from '@ngrx/store/src/tokens';
import * as _ from 'lodash';
import { GrblService } from '../../services/grbl.service';
import { PaperjsComponent } from '../../components/paperjs/paperjs.component';

@Component({
  selector: 'app-console',
  templateUrl: './console.component.html',
  styleUrls: ['./console.component.css'],
  providers: [
    MessageService,
    TerminalService
  ]
})
export class ConsoleComponent implements OnInit, AfterViewInit, OnDestroy {

  subscriptions: Subscription[] = [];

  status: Status;
  configJson: ConfigJson;
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
  responsiveOptions: any;

  constructor(
    private grblService: GrblService,
    private terminalService: TerminalService,
    private storeService: StoreService,
    private messageService: MessageService) {
    this.responsiveOptions = [
      {
        breakpoint: '1024px',
        numVisible: 3,
        numScroll: 3
      },
      {
        breakpoint: '768px',
        numVisible: 2,
        numScroll: 2
      },
      {
        breakpoint: '560px',
        numVisible: 1,
        numScroll: 1
      }
    ];
    // Error handling
    this.subscriptions.push(storeService.getMessages().subscribe(
      (error: any) => {
        this.messageService.add(error);
        if (error.summary === 'Delete ok') {
          this.storeService.dispatchBrowseFiles();
        }
      }
    ));
    // Config store
    this.subscriptions.push(storeService.getConfigJson().subscribe(
      (configJson: ConfigJson) => {
        this.configJson = configJson;
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
        this.files = _.map(_.filter(files, (file: string) => {
          // filter on gcode files
          return file.endsWith('gcode')
        }), (file: string) => {
          return {
            name: file
          }
        });
      }
    ));
    // Emit command from terminal
    this.subscriptions.push(this.terminalService.commandHandler.subscribe(command => {
      this.storeService.dispatchAddCommand({
        forceWrite: command
      });
    }));
  }

  ngOnInit(): void {
    this.storeService.dispatchBrowseFiles();
    this.storeService.dispatchGetConfigJson();
  }

  ngAfterViewInit(): void {
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
  handleRefresh() {
    this.storeService.dispatchBrowseFiles();
  }
  handleDelete(file: any) {
    this.storeService.dispatchDeleteFile(file);
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
    this.storeService.dispatchAddCommand({
      tryWrite: command
    });
  }

  public simulate(value: string) {
    this.grblService.simulate(value).subscribe(
      () => {

      }
    );
  }
}

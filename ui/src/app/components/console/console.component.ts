import { Component, OnInit } from '@angular/core';
import { GrblService } from '../../services/grbl.service';
import { Status } from '../../models/grbl';
import * as paper from 'paper';
import { MessageService } from 'primeng/api';
import { StoreService } from '../../store/store.service';

@Component({
  selector: 'app-console',
  templateUrl: './console.component.html',
  styleUrls: ['./console.component.css'],
  providers: [
    MessageService
  ]
})
export class ConsoleComponent implements OnInit {

  constructor(private storeService: StoreService, private grblService: GrblService, private messageService: MessageService) {
    storeService.getStatus().subscribe(
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
    );
  }

  status: Status;
  step = 1;
  toolsXY: paper.Path.Circle;
  toolsZ: paper.Path.Rectangle;

  ngOnInit(): void {
  }

  refresh(event: any) {
    this.storeService.dispatchSetStatus();
  }

  selectXY(event: any) {
    this.command(`X${event.x} Y${event.y}`);
  }

  drawXY(project: paper.Project) {
    project.activate();
    const rect = new paper.Path.Rectangle({
      from: new paper.Point(0, 0),
      to: new paper.Size(245, 80),
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
    const rect = new paper.Path.Rectangle({
      from: new paper.Point(0, 0),
      to: new paper.Size(80, 30),
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
    this.grblService.setStatus(command).subscribe(
      (result) => {
        this.messageService.add({ severity: 'success', summary: 'Command', detail: `${command}` });
      },
      (error) => {
        this.messageService.add({ severity: 'error', summary: 'Command', detail: `${command}` });
      }
    );
  }
}

import { Component, OnInit } from '@angular/core';
import { GrblService } from 'src/app/services/grbl.service';
import { Status } from 'src/app/models/grbl';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';
import * as paper from 'paper';
import { MessageService } from 'primeng/api';

@Component({
  selector: 'app-console',
  templateUrl: './console.component.html',
  styleUrls: ['./console.component.css'],
  providers: [
    MessageService
  ]
})
export class ConsoleComponent implements OnInit {

  status: Status;
  registerForm: FormGroup;
  submitted = false;
  step = 1;

  private formBuilder = new FormBuilder();

  constructor(private grblService: GrblService, private messageService: MessageService) {
    this.grblService.getStatus().subscribe(
      (status) => {
        this.status = status;
      }
    );
  }

  ngOnInit(): void {
    this.registerForm = this.formBuilder.group({
      wmposx: [],
      wmposy: [],
      wmposz: [],
      wwposx: [],
      wwposy: [],
      wwposz: [],
      wm: [],
      wa: [],
      smposx: [],
      smposy: [],
      smposz: [],
      swposx: [],
      swposy: [],
      swposz: [],
      sm: [],
      sa: [],
      acceptTerms: [false, Validators.requiredTrue]
    }, {
    });
  }

  // convenience getter for easy access to form fields
  get f() { return this.registerForm.controls; }

  onSubmit() {
    this.submitted = true;

    // stop here if form is invalid
    if (this.registerForm.invalid) {
      return;
    }
  }

  refresh(event: any) {
    this.grblService.getStatus().subscribe(
      (status) => {
        this.status = status;
      }
    );
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

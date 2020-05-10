import { Component, OnInit } from '@angular/core';
import { GrblService } from 'src/app/services/grbl.service';
import { Status } from 'src/app/models/grbl';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';

@Component({
  selector: 'app-console',
  templateUrl: './console.component.html',
  styleUrls: ['./console.component.css']
})
export class ConsoleComponent implements OnInit {

  status: Status;
  registerForm: FormGroup;
  submitted = false;

  private formBuilder = new FormBuilder();

  constructor(private grblService: GrblService) {
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

  zoomin(event: any) {
  }

  zoomout(event: any) {
  }

  select(event: any) {
    if (event.event === 'move') {
      this.command(`X${event.x} Y${event.y}`);
    }
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
  setLeft(event: any) {
  }
  setRight(event: any) {
  }
  setBack(event: any) {
  }
  setFront(event: any) {
  }
  setStep(event: any) {
  }
  setUp(event: any) {
  }
  setDown(event: any) {
  }
  setOk(event: any) {
    this.command('ok');
  }

  private command(command: string) {
    this.grblService.setStatus(command).subscribe(
      (result) => {

      }
    );
  }
}

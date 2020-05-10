import { Component, OnInit } from '@angular/core';
import { GrblService } from 'src/app/services/grbl.service';
import { Status } from 'src/app/models/grbl';

@Component({
  selector: 'app-console',
  templateUrl: './console.component.html',
  styleUrls: ['./console.component.css']
})
export class ConsoleComponent implements OnInit {

  status: Status;

  constructor(private grblService: GrblService) {
    this.grblService.getStatus().subscribe(
      (status) => {
        this.status = status;
      }
    );
  }

  ngOnInit(): void {
  }

}

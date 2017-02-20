import { Component, OnInit } from '@angular/core';
import {Observable} from "rxjs/Rx";
import {DateService} from "../../../service/date.service";

@Component({
  selector: 'mr-main-status',
  templateUrl: './main-status.component.html',
  styleUrls: ['./main-status.component.scss']
})
export class MainStatusComponent implements OnInit {

  constructor(public dateService : DateService) { }

  ngOnInit() {
  }
}

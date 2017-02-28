import { Component, OnInit } from '@angular/core';
import {DialService} from "../../service/dial.service";

@Component({
  selector: 'mr-on-call',
  templateUrl: './on-call.component.html',
  styleUrls: ['./on-call.component.scss']
})
export class OnCallComponent implements OnInit {

  constructor(public dialService : DialService) { }

  ngOnInit() {

  }

  onStopCall() {
    this.dialService.stopCall();
  }
}

import { Component, OnInit } from '@angular/core';
import {DialService} from "../service/dial.service";

@Component({
  selector: 'mr-phone-view',
  templateUrl: './phone-view.component.html',
  styleUrls: ['./phone-view.component.scss']
})
export class PhoneViewComponent implements OnInit {

  constructor(public dialService : DialService) { }

  ngOnInit() {
  }

}

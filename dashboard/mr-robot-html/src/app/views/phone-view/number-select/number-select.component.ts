import { Component, OnInit } from '@angular/core';
import {DialService} from "../../service/dial.service";

@Component({
  selector: 'mr-number-select',
  templateUrl: './number-select.component.html',
  styleUrls: ['./number-select.component.scss']
})
export class NumberSelectComponent implements OnInit {

  constructor(public dialService : DialService) { }

  ngOnInit() {
  }

  public onNumberSelected(val : string) {
    this.dialService.setDialNumber(val);
    this.dialService.disableShouldSelectNumber();
  }
}

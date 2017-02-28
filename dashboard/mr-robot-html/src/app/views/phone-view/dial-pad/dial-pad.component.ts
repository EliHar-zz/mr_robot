import {Component, OnInit, Input} from '@angular/core';
import {DialService} from "../../service/dial.service";

@Component({
  selector: 'mr-dial-pad',
  templateUrl: './dial-pad.component.html',
  styleUrls: ['./dial-pad.component.scss']
})
export class DialPadComponent implements OnInit {

  constructor(public dialService : DialService) { }

  ngOnInit() {

  }

  public removeLastChar() {
    if(this.dialService.getDialNumber.length > 0) {
      this.dialService.setDialNumber(
        this.dialService.getDialNumber.substring(0,
          this.dialService.getDialNumber.length-1));
    }
  }

  public appendValue(val : string) {
    this.dialService.setDialNumber(this.dialService.getDialNumber+val);
  }

  public dial() {
    if(this.dialService.getDialNumber.trim().length > 0) {
      this.dialService.startCall();
    }
  }
}

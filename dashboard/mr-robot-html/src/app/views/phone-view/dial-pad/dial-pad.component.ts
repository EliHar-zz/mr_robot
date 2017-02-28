import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'mr-dial-pad',
  templateUrl: './dial-pad.component.html',
  styleUrls: ['./dial-pad.component.scss']
})
export class DialPadComponent implements OnInit {

  public screenValue : string;
  constructor() { }

  ngOnInit() {
    this.screenValue = "";
  }

  public removeLastChar() {
    if(this.screenValue.length > 0) {
      this.screenValue = this.screenValue.substring(0,this.screenValue.length-1);
    }
  }

  public appendValue(val : string) {
    this.screenValue += val;
  }
}

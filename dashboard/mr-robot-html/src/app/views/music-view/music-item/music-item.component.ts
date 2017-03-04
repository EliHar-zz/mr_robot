import { Component, OnInit } from '@angular/core';
import {ActivatedRoute, Params, Router} from "@angular/router";
import {MusicItem} from "./music-item";
import {MusicService} from "../../service/music.service";
import {isNumber} from "util";

@Component({
  selector: 'mr-music-item',
  templateUrl: './music-item.component.html',
  styleUrls: ['./music-item.component.scss']
})
export class MusicItemComponent implements OnInit {

  public id : number;
  public nextId:number;
  public prevId:number;
  public song : MusicItem;
  constructor(private musicService : MusicService, private route: ActivatedRoute) { }

  ngOnInit() {
    this.song = null;
    this.route.params.subscribe((params : Params) => {
        this.id = +params["index"];
        if(!isNaN(this.id)) {
          this.song = this.musicService.getSong(this.id);
          this.nextId = (this.id + 1) % this.musicService.getSongs.length;
          this.prevId = (this.id + this.musicService.getSongs.length - 1) % this.musicService.getSongs.length;
        }
      }
    );
  }
}
